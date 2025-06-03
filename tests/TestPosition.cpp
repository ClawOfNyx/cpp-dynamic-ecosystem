#include "catch2/catch_test_macros.hpp"
#include "Position.h"
#include <vector>

TEST_CASE("Position basic functionality", "[Position]") {
    SECTION("Position construction and getters") {
        Position pos(5, 10);
        REQUIRE(pos.getX() == 5);
        REQUIRE(pos.getY() == 10);
    }
    
    SECTION("Position setters") {
        Position pos(0, 0);
        pos.setX(15);
        pos.setY(20);
        REQUIRE(pos.getX() == 15);
        REQUIRE(pos.getY() == 20);
    }
    
    SECTION("Position copy constructor") {
        Position original(7, 9);
        Position copy(original);
        REQUIRE(copy.getX() == 7);
        REQUIRE(copy.getY() == 9);
        
        copy.setX(100);
        REQUIRE(original.getX() == 7);
        REQUIRE(copy.getX() == 100);
    }
    
    SECTION("Position assignment operator") {
        Position pos1(3, 4);
        Position pos2(8, 12);
        
        pos1 = pos2;
        REQUIRE(pos1.getX() == 8);
        REQUIRE(pos1.getY() == 12);
        
        pos2.setY(50);
        REQUIRE(pos1.getY() == 12);
        REQUIRE(pos2.getY() == 50);
    }
    
    SECTION("Self-assignment") {
        Position pos(10, 15);
        pos = pos;
        REQUIRE(pos.getX() == 10);
        REQUIRE(pos.getY() == 15);
    }
}

TEST_CASE("Position distance calculation", "[Position]") {
    SECTION("Distance to same position") {
        Position pos1(5, 5);
        Position pos2(5, 5);
        REQUIRE(pos1.distanceToPoint(pos2) == 0);
    }
    
    SECTION("Horizontal distance") {
        Position pos1(0, 0);
        Position pos2(3, 0);
        REQUIRE(pos1.distanceToPoint(pos2) == 3);
    }
    
    SECTION("Vertical distance") {
        Position pos1(0, 0);
        Position pos2(0, 4);
        REQUIRE(pos1.distanceToPoint(pos2) == 4);
    }
    
    SECTION("Diagonal distance") {
        Position pos1(0, 0);
        Position pos2(3, 4);
        REQUIRE(pos1.distanceToPoint(pos2) == 5); // 3-4-5 triangle
    }
    
    SECTION("Distance is symmetric") {
        Position pos1(2, 3);
        Position pos2(5, 7);
        REQUIRE(pos1.distanceToPoint(pos2) == pos2.distanceToPoint(pos1));
    }
}

TEST_CASE("Position adjacent positions", "[Position]") {
    SECTION("Center position has 8 adjacent positions") {
        Position center(5, 5);
        std::vector<Position> adjacent = center.getAdjacentPositions();
        REQUIRE(adjacent.size() == 8);
        
        std::vector<std::pair<int, int>> expected = {
            {4, 4}, {4, 5}, {4, 6},
            {5, 4},         {5, 6},
            {6, 4}, {6, 5}, {6, 6}
        };
        
        for (const auto& exp : expected) {
            bool found = false;
            for (const auto& adj : adjacent) {
                if (adj.getX() == exp.first && adj.getY() == exp.second) {
                    found = true;
                    break;
                }
            }
            REQUIRE(found);
        }
    }
    
    SECTION("Corner position filters negative coordinates") {
        Position corner(0, 0);
        std::vector<Position> adjacent = corner.getAdjacentPositions();
        
        REQUIRE(adjacent.size() == 3);
        
        for (const auto& pos : adjacent) {
            REQUIRE(pos.getX() >= 0);
            REQUIRE(pos.getY() >= 0);
        }
        
        std::vector<std::pair<int, int>> expected = {{0, 1}, {1, 0}, {1, 1}};
        for (const auto& exp : expected) {
            bool found = false;
            for (const auto& adj : adjacent) {
                if (adj.getX() == exp.first && adj.getY() == exp.second) {
                    found = true;
                    break;
                }
            }
            REQUIRE(found);
        }
    }
    
    SECTION("Edge position filters negative coordinates") {
        Position edge(0, 5);
        std::vector<Position> adjacent = edge.getAdjacentPositions();
        
        REQUIRE(adjacent.size() == 5);
        
        for (const auto& pos : adjacent) {
            REQUIRE(pos.getX() >= 0);
            REQUIRE(pos.getY() >= 0);
        }
    }
}