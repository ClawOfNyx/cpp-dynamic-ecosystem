#include "catch2/catch_test_macros.hpp"
#include "Tile.h"
#include "Grid.h"
#include "Plant.h"
#include "Animal.h"
#include <stdexcept>

TEST_CASE("Tile basic functionality", "[Tile]") {
    Position pos(5, 10);
    
    SECTION("Empty tile initialization") {
        Tile tile(pos);
        REQUIRE(tile.isEmpty());
        REQUIRE(tile.getOccupant() == nullptr);
        REQUIRE(tile.getPosition().getX() == 5);
        REQUIRE(tile.getPosition().getY() == 10);
    }
    
    SECTION("Setting and getting occupant") {
        Tile tile(pos);
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        
        tile.setOccupant(*plant);
        REQUIRE_FALSE(tile.isEmpty());
        REQUIRE(tile.getOccupant() != nullptr);
        REQUIRE(tile.getOccupant()->getType() == OrganismType::PLANT);
        
        delete plant;
    }
    
    SECTION("Clearing occupant") {
        Tile tile(pos);
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        
        tile.setOccupant(*plant);
        REQUIRE_FALSE(tile.isEmpty());
        
        tile.clearOccupant();
        REQUIRE(tile.isEmpty());
        REQUIRE(tile.getOccupant() == nullptr);
        
        delete plant;
    }
}

TEST_CASE("Grid construction and bounds checking", "[Grid]") {
    SECTION("Valid grid construction") {
        Grid grid(10, 15);
        REQUIRE(grid.isInBounds(0, 0));
        REQUIRE(grid.isInBounds(9, 14));
        REQUIRE_FALSE(grid.isInBounds(-1, 0));
        REQUIRE_FALSE(grid.isInBounds(0, -1));
        REQUIRE_FALSE(grid.isInBounds(10, 0));
        REQUIRE_FALSE(grid.isInBounds(0, 15));
    }
    
    SECTION("Invalid grid construction") {
        REQUIRE_THROWS_AS(Grid(-1, 10), std::invalid_argument);
        REQUIRE_THROWS_AS(Grid(10, -1), std::invalid_argument);
        REQUIRE_THROWS_AS(Grid(0, 10), std::invalid_argument);
        REQUIRE_THROWS_AS(Grid(10, 0), std::invalid_argument);
    }
}

TEST_CASE("Grid tile access", "[Grid]") {
    Grid grid(5, 5);
    
    SECTION("Valid tile access") {
        Tile& tile = grid.getTile(2, 3);
        REQUIRE(tile.getPosition().getX() == 2);
        REQUIRE(tile.getPosition().getY() == 3);
        REQUIRE(tile.isEmpty());
    }
    
    SECTION("Out of bounds tile access throws exception") {
        REQUIRE_THROWS_AS(grid.getTile(-1, 0), std::out_of_range);
        REQUIRE_THROWS_AS(grid.getTile(0, -1), std::out_of_range);
        REQUIRE_THROWS_AS(grid.getTile(5, 0), std::out_of_range);
        REQUIRE_THROWS_AS(grid.getTile(0, 5), std::out_of_range);
    }
}

TEST_CASE("Grid finding closest empty tile", "[Grid]") {
    Grid grid(5, 5);
    
    SECTION("Find closest empty tile in empty grid") {
        Position center(2, 2);
        Tile& closest = grid.findClosestEmptyTile(center);
        
        REQUIRE(closest.isEmpty());
        Position closestPos = closest.getPosition();
        REQUIRE(grid.isInBounds(closestPos.getX(), closestPos.getY()));
    }
    
    SECTION("Find closest empty tile with some occupied tiles") {
        // Fill center tile
        Position centerPos(2, 2);
        Plant* centerPlant = new Plant(10.0f, 100, 0.5f, 0.3f);
        Tile& centerTile = grid.getTile(2, 2);
        centerTile.setOccupant(*centerPlant);
        
        Position searchFrom(2, 2);
        Tile& closest = grid.findClosestEmptyTile(searchFrom);
        
        REQUIRE(closest.isEmpty());
        Position closestPos = closest.getPosition();
        int dx = abs(searchFrom.getX() - closestPos.getX());
        int dy = abs(searchFrom.getY() - closestPos.getY());
        int distance = dx + dy; 
        REQUIRE(distance >= 1); 
        
        delete centerPlant;
    }
    
    SECTION("No empty tiles throws exception") {
        // Fill all tiles
        std::vector<Plant*> plants;
        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                Plant* plant = new Plant(5.0f, 50, 0.3f, 0.2f);
                plants.push_back(plant);
                Tile& tile = grid.getTile(x, y);
                tile.setOccupant(*plant);
            }
        }
        
        Position searchFrom(2, 2);
        REQUIRE_THROWS_AS(grid.findClosestEmptyTile(searchFrom), std::runtime_error);
        
        // Clean up
        for (Plant* plant : plants) {
            delete plant;
        }
    }
}

TEST_CASE("Grid finding closest organism", "[Grid]") {
    Grid grid(5, 5);
    
    SECTION("Find closest plant") {
        // Place a plant
        Position plantPos(1, 1);
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        plant->setPosition(plantPos);
        Tile& plantTile = grid.getTile(1, 1);
        plantTile.setOccupant(*plant);
        
        Position searchFrom(0, 0);
        Organism& closest = grid.findClosestOrganism(searchFrom, OrganismType::PLANT);
        
        REQUIRE(closest.getType() == OrganismType::PLANT);
        REQUIRE(closest.getPosition().getX() == 1);
        REQUIRE(closest.getPosition().getY() == 1);
        
        delete plant;
    }
    
    SECTION("Find closest animal") {
        // Place an animal
        Position animalPos(3, 3);
        Animal* animal = new Animal(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 5);
        animal->setPosition(animalPos);
        Tile& animalTile = grid.getTile(3, 3);
        animalTile.setOccupant(*animal);
        
        Position searchFrom(2, 2);
        Organism& closest = grid.findClosestOrganism(searchFrom, OrganismType::ANIMAL);
        
        REQUIRE(closest.getType() == OrganismType::ANIMAL);
        REQUIRE(closest.getPosition().getX() == 3);
        REQUIRE(closest.getPosition().getY() == 3);
        
        delete animal;
    }
    
    SECTION("No organism of type found throws exception") {
        Position searchFrom(2, 2);
        REQUIRE_THROWS_AS(grid.findClosestOrganism(searchFrom, OrganismType::PLANT), std::runtime_error);
    }
    
    SECTION("Find closest among multiple organisms") {
        // Place multiple plants
        Position plant1Pos(1, 1);
        Position plant2Pos(4, 4);
        
        Plant* plant1 = new Plant(10.0f, 100, 0.5f, 0.3f);
        Plant* plant2 = new Plant(10.0f, 100, 0.5f, 0.3f);
        
        plant1->setPosition(plant1Pos);
        plant2->setPosition(plant2Pos);
        
        grid.getTile(1, 1).setOccupant(*plant1);
        grid.getTile(4, 4).setOccupant(*plant2);
        
        Position searchFrom(0, 0);
        Organism& closest = grid.findClosestOrganism(searchFrom, OrganismType::PLANT);
        
        REQUIRE(closest.getPosition().getX() == 1);
        REQUIRE(closest.getPosition().getY() == 1);
        
        delete plant1;
        delete plant2;
    }
}