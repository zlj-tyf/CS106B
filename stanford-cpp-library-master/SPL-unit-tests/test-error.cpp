/*
 * Test file for verifying the Stanford C++ lib error module.
 */

#include "error.h"
#include "vector.h"
#include <csignal>
#include <iostream>
#include <string>
#include "SimpleTest.h"
using namespace std;

namespace MyNS {
    class LoL {
    public:
        static int cCcCc_cCcCc(int, char*, double, char, bool) {
            cout << "function c start" << endl;
            cout << endl;
            // error("blargh!");
            Vector<int> v;
            v.get(42);  // lolol
            cout << "function c end" << endl;
            return 0;
        }
    };
}

void b() {
    cout << "function b start" << endl;
    MyNS::LoL::cCcCc_cCcCc(0, nullptr, 0, 0, false);
    cout << "function b end" << endl;
}

void a() {
    cout << "function a start" << endl;
    b();
    cout << "function a end" << endl;
}

void exceptionTest() {
    a();
}


// commenting out because of compiler warning about infinite recursion
void stackOverflowTest(int n) {
    int a[100] = {0};
    if (a[0] || n % 1000 == 0) {
        cout << "stack overflow n=" << n << endl;
    }
    if (!a[0]) {
        stackOverflowTest(n+1);
    }
}

void segC_theOneThatActuallyThrows(int sig) {
    if (sig == SIGFPE) {
        // divide by 0 (generate SIGFPE)
        int a = 1;
        int b = 0;
        cout << (a/b) << endl;
    } else if (sig == SIGSEGV) {
        // dereference a NULL pointer (generate SIGSEGV)
        int* foo = nullptr;
        cout << *foo << endl;
    } else if (sig == SIGABRT) {
        // cause program to "abort" (generate SIGABRT)
        abort();
    }
    cout << "will never get here lol" << endl;
}

void segB(int sig) {
    segC_theOneThatActuallyThrows(sig);
}

void segA(int sig) {
    segB(sig);
}

void raiseSignal(int sig) {
    if (sig == 0) {
        sig = SIGSEGV;
    }
    cout << "Hello, world!" << endl;
    segA(sig);
}

void excepB() {
    int x = 42;
    string s = "hi";
    s += x;
    throw 42;
}

void excepA() {
    int x = 42;
    string s = "hi";
    s += x;
    excepB();
}


PROVIDED_TEST("error raised under EXPECT_ERROR should report as success") {
    EXPECT_ERROR(exceptionTest());
}

PROVIDED_TEST("error raised under EXPECT_NO_ERROR should report as failure (HEADS UP: text expected to fail)") {
    EXPECT_NO_ERROR(exceptionTest());
}

PROVIDED_TEST("throw std::exception under EXPECT_ERROR should report as failure (HEADS UP: test expected to fail)") {
    EXPECT_ERROR(throw std::exception());
}

PROVIDED_TEST("throw std::string under EXPECT_ERROR should report as failure (HEADS UP: test expected to fail)") {
    EXPECT_ERROR(throw string("oh crap"));
}



#if 0

PROVIDED_TEST("call std::exit during test") {
    exit(0);
}

PROVIDED_TEST("Exceptions, seg fault (should crash)") {
    raiseSignal(SIGSEGV);
}

PROVIDED_TEST("Exceptions, divide by zero (should crash)") {
   raiseSignal(SIGFPE);
}

PROVIDED_TEST("Exceptions, AbortTest") {
    raiseSignal(SIGABRT);
}

// we currently can't gracefully catch stack overflows
PROVIDED_TEST("Exceptions, StackOverflowTest") {
    stackOverflowTest(42);
}
#endif
