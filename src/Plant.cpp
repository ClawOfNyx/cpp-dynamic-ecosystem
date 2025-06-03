#include "Plant.h"
#include "Grid.h"
#include "WorldManager.h"  // ADD THIS LINE
#include <random>
#include <vector>
#include <iostream>

Plant::Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate, float spreadingThreshold)
    : Organism(OrganismType::PLANT, nutrients, maxLifespan),
      growthRate(growthRate), 
      nutrientAbsorptionRate(nutrientAbsorptionRate),
      spreadingThreshold(spreadingThreshold) {}

float Plant::getGrowthRate() const {
    return growthRate;
}

float Plant::getNutrientAbsorptionRate() const {
    return nutrientAbsorptionRate;
}

void Plant::setGrowthRate(float rate) {
    growthRate = rate;
}

void Plant::setNutrientAbsorptionRate(float rate) {
    nutrientAbsorptionRate = rate;
}

void Plant::update(Grid& grid, WorldManager& worldManager) {
    absorbNutrients();
    incrementAge();
    
    // Try to spread if ready to reproduce
    if (isReadyToReproduce()) {
        trySpread(grid, worldManager);
    }
}

bool Plant::isReadyToReproduce() const {
    return nutrients > spreadingThreshold;
}

void Plant::consumeResources() { }

Organism* Plant::reproduce() {
    if (!isReadyToReproduce()) {
        return nullptr;
    }
    
    consumeNutrients(spreadingThreshold / 2);
    
    // Create offspring at same position - will be moved by trySpread
    return new Plant(
        spreadingThreshold / 2,
        maxLifespan,
        growthRate,
        nutrientAbsorptionRate,
        spreadingThreshold  // Pass the same spreading threshold to offspring
    );
}

void Plant::absorbNutrients() {
    addNutrients(nutrientAbsorptionRate * growthRate);
}

void Plant::trySpread(Grid& grid, WorldManager& worldManager) {
    if (!isReadyToReproduce()) {
        return; // Early exit if not ready
    }
    
    std::vector<Position> adjacentPositions = position->getAdjacentPositions();
    std::vector<Position> validPositions;
    
    for (const auto& pos : adjacentPositions) {
        if (grid.isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid.getTile(pos.getX(), pos.getY());
            if (tile.isEmpty()) {
                validPositions.push_back(pos);
            }
        }
    }
    
    if (!validPositions.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, validPositions.size() - 1);
        
        Position spreadPos = validPositions[dis(gen)];
        Plant* offspring = dynamic_cast<Plant*>(reproduce());
        
        if (offspring) {
            worldManager.addOrganism(offspring, spreadPos);
            std::cout << "Plant spread to (" << spreadPos.getX() << ", " << spreadPos.getY() << ")" << std::endl;
        }
    }
}