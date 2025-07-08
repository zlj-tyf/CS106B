/*
 * Test file for verifying the Stanford C++ lib Map class.
 */

#include "map.h"
#include "hashset.h"
#include "set.h"
#include "common.h"
#include "SimpleTest.h"
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>

/*
 * Force instantiation of Map on a few types to make sure we didn't miss anything.
 * The key types must be comparable.
 */
template class Map<int, int>;
template class Map<std::string, int>;

/*
 * Uncomment this code to include tests that the nice error messages for types missing
 * hashing show up properly.
 */
#if 0
void causeCompilerError() {
    struct Bad {};

    Map<Bad, int> bad; // Should trigger a static assertion rather than a long chain of sorrows
}
#endif

/* This code, on the other hand, should NOT cause a compiler error, since we gave an
 * explicit comparison function.
 */
void customComparatorNoError() {
    struct Meh {};

    Map<Meh, int> okay([](const Meh&, const Meh&) {
        return true;
    });

    (void) okay;
}

PROVIDED_TEST("Map, compare") {
    // TODO
}

PROVIDED_TEST("Map, forEach") {
    Map<std::string, int> map;
    map.put("zz", 26);
    map.put("a", 1);
    map.put("ss", 97);
    map.put("bbbb", 2);
    map.put("E", 4);
    map.put("yZ44", 33);
    Set<std::string> expected {"zz", "a", "ss", "bbbb", "E", "yZ44"};
    for (const std::string& key : map) {
        EXPECT(expected.contains(key));
    }
    map.remove("ss");
    map.remove("zz");
    map.remove("yZ44");
    map.remove("notfound");
    expected = {"a", "bbbb", "E"};
    for (const std::string& key : map) {
        EXPECT(expected.contains(key));
    }
}

PROVIDED_TEST("Map, firstLast") {
    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    EXPECT_EQUAL(map.firstKey(), "a");
    EXPECT_EQUAL(map.lastKey(),  "c");
}

PROVIDED_TEST("Map, hashCode") {
    Map<int, int> map;
    map.put(69, 96);
    map.put(42, 24);
    EXPECT_EQUAL( hashCode(map), hashCode(map));

    Map<int, int> copy = map;
    EXPECT_EQUAL( hashCode(map), hashCode(copy));

    Map<int, int> empty;

    HashSet<Map<int, int> > hashmap {map, copy, empty, empty};
    EXPECT_EQUAL( 2, hashmap.size());
}

PROVIDED_TEST("Map, initializerList") {
    std::initializer_list<std::pair<const std::string, int>> pairlist = {{"k", 60}, {"t", 70}};
    std::initializer_list<std::pair<const std::string, int>> pairlist2 = {{"b", 20}, {"e", 50}};

    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}};
    EXPECT_EQUAL( 10, map.get("a"));
    EXPECT_EQUAL( 20, map.get("b"));
    EXPECT_EQUAL( 30, map.get("c"));
    EXPECT_EQUAL( 3, map.size());

    map += {{"d", 40}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int>> expected1 = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after +=", expected1, map);

    Map<std::string, int> copy = map + pairlist;
    std::initializer_list<std::pair<const std::string, int>> expected2 = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after + (shouldn't modify)", expected2, map);
    // std::initializer_list<std::pair<const std::string, int>> expected3 = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"k", 60}, {"t", 70}};
    // assertMap("after + copy", expected, copy);

    copy = map - pairlist2;
    std::initializer_list<std::pair<const std::string, int>> expected4 = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after - (shouldn't modify)", expected4, map);
    std::initializer_list<std::pair<const std::string, int>> expected5 = {{"a", 10}, {"c", 30}, {"d", 40}};
    assertMap("after - copy", expected5, copy);

    copy = map * pairlist2;
    std::initializer_list<std::pair<const std::string, int>> expected6 = {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}};
    assertMap("after * (shouldn't modify)", expected6, map);
    std::initializer_list<std::pair<const std::string, int>> expected7 = {{"b", 20}, {"e", 50}};
    assertMap("after * copy", expected7, copy);

    map -= {{"d", 40}, {"e", 50}};
    std::initializer_list<std::pair<const std::string, int>> expected8 = {{"a", 10}, {"b", 20}, {"c", 30}};
    assertMap("after -=", expected8, map);

    map *= pairlist2;
    std::initializer_list<std::pair<const std::string, int>> expected9 = {{"b", 20}};
    assertMap("after *=", expected9, map);
}

void addDuring(Map<std::string, int>& m) { for (auto k:m) m.put(k+k, 3); }
void removeDuring(Map<std::string, int> & m) { for (auto k:m) m.remove(k); }

PROVIDED_TEST("Map, error on modify during iterate") {
    Map<std::string, int> map {{"a", 10}, {"b", 20}, {"c", 30}, {"d", 40}, {"e", 50}, {"f", 60}};
    EXPECT_ERROR(addDuring(map));
    EXPECT_ERROR(removeDuring(map));
}

PROVIDED_TEST("Map, randomKey") {
    Map<std::string, int> counts;
    int RUNS = 200;

    std::initializer_list<std::string> list {"a", "b", "c", "d", "e", "f"};

    Map<std::string, int> map;
    map["a"] = 50;
    map["b"] = 40;
    map["c"] = 30;
    map["d"] = 20;
    map["e"] = 10;
    map["f"] =  0;
    for (int i = 0; i < RUNS; i++) {
        std::string s = randomKey(map);
        counts[s]++;
    }

    for (const std::string& s : list) {
        EXPECT(counts[s] > 0);
    }
}

PROVIDED_TEST("Map, streamExtract") {
    std::istringstream stream("{2:20, 1:10, 4:40, 3:30}");
    Map<int, int> map;
    stream >> map;
    EXPECT_EQUAL(map.toString(), "{1:10, 2:20, 3:30, 4:40}");
}

PROVIDED_TEST("Map, operator[]") {
    const Map<std::string, bool> m1 = {
        { "A", true }
    };
    const Map<std::string, int> m2 = {
        { "A", 1 }
    };
    const Map<std::string, float> m3 = {
        { "A", 1.0f }
    };
    const Map<std::string, double> m4 = {
        { "A", 1.0 }
    };

    auto ptr = std::make_shared<std::istringstream>();
    const Map<std::string, std::shared_ptr<std::istringstream>> m5 = {
        { "A", ptr }
    };

    EXPECT_EQUAL(m1["A"], true);
    EXPECT_EQUAL(m1["B"], false);

    EXPECT_EQUAL(m2["A"], 1);
    EXPECT_EQUAL(m2["B"], 0);

    EXPECT_EQUAL(m3["A"], 1.0f);
    EXPECT_EQUAL(m3["B"], 0.0f);

    EXPECT_EQUAL(m4["A"], 1.0);
    EXPECT_EQUAL(m4["B"], 0.0);

    EXPECT_EQUAL(m5["A"], ptr);
    EXPECT_EQUAL(m5["B"], nullptr);
}
