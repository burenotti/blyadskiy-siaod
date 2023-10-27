#define CATCH_CONFIG_MAIN

#include <catch2/catch2.hpp>
#include <hash_map.hpp>
#include <string>

using namespace burenotti::hash_map;

TEST_CASE("operator[] can insert values without collisions", "[HashMap]") {

    HashMap<std::string, int> map;
    std::hash<std::string> hasher;

    REQUIRE(hasher("answer") % 16 != hasher("offer") % 16);
    map["answer"] = 42;
    map["offer"] = 69;

    REQUIRE(map.contains("answer"));
    REQUIRE(map.contains("offer"));
    REQUIRE(map["answer"] == 42);
    REQUIRE(map["offer"] == 69);

}


TEST_CASE("operator[] can insert values with collisions", "[HashMap]") {
    std::hash<int> hasher;

    REQUIRE(hasher(0) % 32 == hasher(32) % 32);

    HashMap<int, int> map(32);
    map[0] = 42;
    REQUIRE(map.size() == 1);
    map[32] = 69;
    REQUIRE(map.size() == 2);

    REQUIRE(map.contains(0));
    REQUIRE(map.contains(32));
    REQUIRE(map[0] == 42);
    REQUIRE(map[32] == 69);

}

TEST_CASE("can grow", "[HashMap]") {
    HashMap<int, int> map;
    for (int i = 0; i < 1000; i++) {
        map[i] = i * 2;
    }
    REQUIRE(map.size() == 1000);
}

TEST_CASE("can erase values from map", "[HashMap]") {
    HashMap<int, int> map;
    map[0] = 1;
    map[15] = 2;
    map[16] = 3;
    REQUIRE(map.size() == 3);
    map.erase(0);
    REQUIRE_FALSE(map.contains(0));
    REQUIRE(map.size() == 2);


    map.erase(15);
    REQUIRE_FALSE(map.contains(15));
    REQUIRE(map.size() == 1);

    map.erase(16);
    REQUIRE_FALSE(map.contains(16));
    REQUIRE(map.size() == 0);
}
