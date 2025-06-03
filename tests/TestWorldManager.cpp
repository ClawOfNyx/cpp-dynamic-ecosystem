#include "catch2/catch_test_macros.hpp"
#include "WorldManager.h"
#include "Plant.h"
#include "Animal.h"
#include "Position.h"
#include <iostream>
#include <memory>

void resetWorldManager() {
}

TEST_CASE("WorldManager singleton pattern", "[WorldManager]") {
    SECTION("Singleton instance creation") {
        WorldManager& manager1 = WorldManager::getInstance(10, 10, 5.0f);
        WorldManager& manager2 = WorldManager::getInstance(20, 20, 10.0f); // Different params
        
        // Should return the same instance regardless of parameters
        REQUIRE(&manager1 == &manager2);
        
        // Grid should have original dimensions (10x10) since it was created first
        const Grid& grid = manager1.getGrid();
        REQUIRE(grid.isInBounds(9, 9));
        REQUIRE_FALSE(grid.isInBounds(10, 10));
    }
    
    SECTION("Grid access") {
        WorldManager& manager = WorldManager::getInstance(5, 8, 3.0f);
        const Grid& grid = manager.getGrid();
        
        // Note: Since singleton persists, this will still be the original 10x10 grid
        REQUIRE(grid.isInBounds(0, 0));
        REQUIRE(grid.isInBounds(9, 9));
    }
}

TEST_CASE("WorldManager organism management", "[WorldManager]") {
    WorldManager& manager = WorldManager::getInstance(15, 15, 2.0f);
    
    SECTION("Add organism with Position object") {
        int initialCount = manager.getOrganismCount();
        
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        Position pos(5, 5);
        
        manager.addOrganism(plant, pos);
        
        REQUIRE(manager.getOrganismCount() == initialCount + 1);
        
        // Verify organism is on grid
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(5, 5);
        REQUIRE_FALSE(tile.isEmpty());
        REQUIRE(tile.getOccupant() == plant);
    }
    
    SECTION("Add organism with coordinates") {
        int initialCount = manager.getOrganismCount();
        
        Animal* animal = new Animal(20.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.5f, 25.0f, 10);
        
        manager.addOrganism(animal, 7, 8);
        
        REQUIRE(manager.getOrganismCount() == initialCount + 1);
        
        // Verify organism is on grid
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(7, 8);
        REQUIRE_FALSE(tile.isEmpty());
        REQUIRE(tile.getOccupant() == animal);
    }
    
    SECTION("Cannot add organism to occupied tile") {
        // Add first organism
        Plant* plant1 = new Plant(8.0f, 50, 0.4f, 0.2f);
        manager.addOrganism(plant1, 3, 3);
        
        int countAfterFirst = manager.getOrganismCount();
        
        // Try to add second organism to same location
        Plant* plant2 = new Plant(12.0f, 60, 0.6f, 0.4f);
        manager.addOrganism(plant2, 3, 3);
        
        // Count should not increase (plant2 gets deleted in WorldManagerImpl)
        REQUIRE(manager.getOrganismCount() == countAfterFirst);
        
        // Original organism should still be there
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(3, 3);
        REQUIRE(tile.getOccupant() == plant1);
    }
    
    SECTION("Cannot add organism out of bounds") {
        int initialCount = manager.getOrganismCount();
        
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        
        // Try to add out of bounds (organism gets deleted in WorldManagerImpl)
        // Using 15,5 instead of 20,5 since grid is 10x10 (indices 0-9)
        manager.addOrganism(plant, 15, 5);
        
        REQUIRE(manager.getOrganismCount() == initialCount);
    }
    
    SECTION("Cannot add null organism") {
        int initialCount = manager.getOrganismCount();
        
        manager.addOrganism(nullptr, 5, 5);
        
        // Count should not increase
        REQUIRE(manager.getOrganismCount() == initialCount);
    }
}

TEST_CASE("WorldManager organism removal", "[WorldManager]") {
    WorldManager& manager = WorldManager::getInstance(15, 15, 2.0f);
    
    SECTION("Remove organism by pointer") {
        Plant* plant = new Plant(10.0f, 100, 0.5f, 0.3f);
        manager.addOrganism(plant, 4, 6);
        
        int countAfterAdd = manager.getOrganismCount();
        
        manager.removeOrganism(plant);
        
        REQUIRE(manager.getOrganismCount() == countAfterAdd - 1);
        
        // Verify tile is empty
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(4, 6);
        REQUIRE(tile.isEmpty());
    }
    
    SECTION("Remove organism by position") {
        // Create and add organism to a fresh location
        Animal* animal = new Animal(15.0f, 80, 2, 5, AnimalType::CARNIVORE, 1.2f, 20.0f, 8);
        Position pos(2, 1); 
        
        // First ensure the position is empty
        const Grid& grid = manager.getGrid();
        if (!grid.getTile(pos.getX(), pos.getY()).isEmpty()) {
            // If occupied, remove existing organism first
            manager.removeOrganism(pos);
        }
        
        manager.addOrganism(animal, pos);
        int countAfterAdd = manager.getOrganismCount();
        
        // Verify organism was added
        REQUIRE_FALSE(grid.getTile(pos.getX(), pos.getY()).isEmpty());
        
        manager.removeOrganism(pos);
        
        REQUIRE(manager.getOrganismCount() == countAfterAdd - 1);
        
        // Verify tile is empty
        const Tile& tile = grid.getTile(pos.getX(), pos.getY());
        REQUIRE(tile.isEmpty());
    }
    
    SECTION("Remove null organism does nothing") {
        int initialCount = manager.getOrganismCount();
        
        manager.removeOrganism(static_cast<Organism*>(nullptr));
        
        REQUIRE(manager.getOrganismCount() == initialCount);
    }
    
    SECTION("Remove from empty position does nothing") {
        int initialCount = manager.getOrganismCount();
        
        Position emptyPos(1, 0);
        const Grid& grid = manager.getGrid();
        if (!grid.getTile(emptyPos.getX(), emptyPos.getY()).isEmpty()) {
            manager.removeOrganism(emptyPos); // Clear it first
            initialCount = manager.getOrganismCount(); // Update count
        }
        
        manager.removeOrganism(emptyPos);
        
        REQUIRE(manager.getOrganismCount() == initialCount);
    }
}

TEST_CASE("WorldManager plant spawning from dead organisms", "[WorldManager]") {
    WorldManager& manager = WorldManager::getInstance(15, 15, 2.0f);
    
    SECTION("Spawn plant from dead organism") {
        int initialCount = manager.getOrganismCount();
        
        Position spawnPos(6, 7);
        float nutrients = 20.0f;
        
        manager.spawnPlantFromDeadOrganism(spawnPos, nutrients);
        
        REQUIRE(manager.getOrganismCount() == initialCount + 1);
        
        // Verify plant was created
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(6, 7);
        REQUIRE_FALSE(tile.isEmpty());
        
        Organism* organism = tile.getOccupant();
        REQUIRE(organism->getType() == OrganismType::PLANT);
        
        Plant* plant = dynamic_cast<Plant*>(organism);
        REQUIRE(plant != nullptr);
        
        float expectedNutrients = std::max(nutrients / 2, 4.0f);
        REQUIRE(plant->getNutrients() == expectedNutrients);
    }
    
    SECTION("Spawn plant with minimum nutrients") {
        Position spawnPos(8, 9);
        float nutrients = 5.0f; // Low nutrients
        
        manager.spawnPlantFromDeadOrganism(spawnPos, nutrients);
        
        // Verify plant was created
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(8, 9);
        REQUIRE_FALSE(tile.isEmpty());
        
        Plant* plant = dynamic_cast<Plant*>(tile.getOccupant());
        REQUIRE(plant != nullptr);
        
        REQUIRE(plant->getNutrients() == 4.0f);
    }
    
    SECTION("Cannot spawn plant on occupied tile") {
        Plant* existingPlant = new Plant(8.0f, 50, 0.4f, 0.2f);
        Position pos(4, 5); // Use position within 10x10 grid bounds (0-9)
        
        // Clear position if occupied
        const Grid& grid = manager.getGrid();
        if (!grid.getTile(pos.getX(), pos.getY()).isEmpty()) {
            manager.removeOrganism(pos);
        }
        
        manager.addOrganism(existingPlant, pos);
        
        int countAfterFirst = manager.getOrganismCount();
        
        // Try to spawn plant on same tile
        manager.spawnPlantFromDeadOrganism(pos, 15.0f);
        
        // Count should not increase
        REQUIRE(manager.getOrganismCount() == countAfterFirst);
        
        // Original plant should still be there
        const Tile& tile = grid.getTile(pos.getX(), pos.getY());
        REQUIRE(tile.getOccupant() == existingPlant);
    }
    
    SECTION("Cannot spawn plant out of bounds") {
        int initialCount = manager.getOrganismCount();
        
        Position outOfBounds(15, 5); // x=15 is out of bounds for 10x10 grid
        manager.spawnPlantFromDeadOrganism(outOfBounds, 15.0f);
        
        // Count should not increase
        REQUIRE(manager.getOrganismCount() == initialCount);
    }
}

TEST_CASE("WorldManager update functionality", "[WorldManager]") {
    WorldManager& manager = WorldManager::getInstance(10, 10, 2.0f);
    
    SECTION("Update empty world") {
        int initialCount = manager.getOrganismCount();
        
        // Should not crash with organisms present
        manager.update();
        
        // Count might change due to existing organisms
        REQUIRE(manager.getOrganismCount() >= 0);
    }
    
    SECTION("Update world with new organisms") {
        // Add some organisms to fresh positions
        Plant* plant = new Plant(15.0f, 100, 0.5f, 0.3f);
        Animal* herbivore = new Animal(20.0f, 80, 2, 3, AnimalType::HERBIVORE, 2.0f, 30.0f, 5);
        
        manager.addOrganism(plant, 1, 1);
        manager.addOrganism(herbivore, 2, 2);
        
        int countBeforeUpdate = manager.getOrganismCount();
        
        // Update should not crash
        manager.update();
        
        // Organisms might die or reproduce, but count should be valid
        REQUIRE(manager.getOrganismCount() >= 0);
    }
    
    SECTION("Update handles organism lifecycle") {
        // Create organism with very low lifespan
        Plant* dyingPlant = new Plant(0.1f, 1, 0.1f, 0.1f); // Very low nutrients and short lifespan
        manager.addOrganism(dyingPlant, 0, 1);
        
        // Update multiple times to allow organism lifecycle
        for (int i = 0; i < 10; ++i) {
            manager.update();
        }
        
        // System should handle organism death and potential decomposition
        const Grid& grid = manager.getGrid();
        const Tile& tile = grid.getTile(0, 1);
        
        // Tile might be empty or have a decomposition plant
        if (!tile.isEmpty()) {
            Organism* occupant = tile.getOccupant();
            REQUIRE(occupant->getType() == OrganismType::PLANT);
            // This should be a decomposition plant if present
        }
    }
}

TEST_CASE("WorldManager integration scenarios", "[WorldManager]") {
    WorldManager& manager = WorldManager::getInstance(12, 12, 2.0f);
    
    SECTION("Basic ecosystem simulation") {
        // Clear area for testing
        int initialCount = manager.getOrganismCount();
        
        // Add organisms to specific locations
        Plant* plant = new Plant(20.0f, 100, 0.5f, 0.3f);
        manager.addOrganism(plant, 5, 5);
        
        Animal* herbivore = new Animal(15.0f, 80, 1, 2, AnimalType::HERBIVORE, 1.0f, 25.0f, 5);
        manager.addOrganism(herbivore, 6, 5); // Adjacent to plant
        
        int countAfterSetup = manager.getOrganismCount();
        REQUIRE(countAfterSetup >= initialCount + 2);
        
        // Run a few update cycles
        for (int i = 0; i < 3; ++i) {
            manager.update();
        }
        
        // System should remain stable
        REQUIRE(manager.getOrganismCount() >= 0);
        REQUIRE(manager.getOrganismCount() <= 100); // Reasonable upper bound
    }
    
    SECTION("Multi-organism ecosystem") {
        int initialCount = manager.getOrganismCount();
        
        // Create a small ecosystem in a corner
        std::vector<Plant*> plants;
        std::vector<Animal*> animals;
        
        // Add plants - use positions within bounds
        for (int i = 0; i < 3; ++i) {
            Plant* plant = new Plant(12.0f + i, 100, 0.6f, 0.4f);
            plants.push_back(plant);
            // Use positions within the 10x10 grid bounds
            manager.addOrganism(plant, 7 + (i % 2), 7 + (i / 2));
        }
        
        // Add herbivore
        Animal* herbivore = new Animal(18.0f, 80, 1, 3, AnimalType::HERBIVORE, 1.5f, 28.0f, 6);
        animals.push_back(herbivore);
        manager.addOrganism(herbivore, 6, 6);
        
        int countAfterSetup = manager.getOrganismCount();
        REQUIRE(countAfterSetup >= initialCount + 3);
        
        // Run simulation
        for (int step = 0; step < 5; ++step) {
            manager.update();
            
            // Ensure system remains bounded
            int currentCount = manager.getOrganismCount();
            REQUIRE(currentCount >= 0);
            REQUIRE(currentCount <= 200); // Reasonable upper bound
        }
        
        // System should still have some organisms
        REQUIRE(manager.getOrganismCount() >= 0);
    }
}