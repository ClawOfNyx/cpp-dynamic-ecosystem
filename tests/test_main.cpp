#include <catch2/catch_test_macros.hpp>

int add(int a, int b) {
    return a + b;
}

TEST_CASE("Addition works") {
    REQUIRE(add(2, 3) == 5);
}