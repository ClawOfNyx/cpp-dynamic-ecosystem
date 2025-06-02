#include "Plant.h"
#include "Grid.h"
#include "WorldManager.h"
#include <random>
#include <vector>
#include <iostream>

Plant::Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate)
    : Organism(OrganismType::PLANT, nutrients, maxLifespan),
      growthRate(growthRate), 
      nutrientAbsorptionRate(nutrientAbsorptionRate),
      spreadingThreshold(8.0f) {}

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
    incrementAge();
    
    std::cout << "Plant at (" << position->getX() << ", " << position->getY() 
              << ") has " << nutrients << " nutrients (threshold: " << spreadingThreshold << ")" << std::endl;
    
    if (isReadyToReproduce()) {
        std::vector<Position> adjacentPositions = position->getAdjacentPositions();
        bool canSpread = false;
        
        for (const auto& pos : adjacentPositions) {
            if (grid.isInBounds(pos.getX(), pos.getY())) {
                Tile& tile = grid.getTile(pos.getX(), pos.getY());
                if (tile.isEmpty()) {
                    canSpread = true;
                    break;
                }
            }
        }
        
        if (canSpread) {
            std::cout << "Plant is ready to reproduce!" << std::endl;
            trySpread(grid, worldManager);
            return; 
        }
    }
    absorbNutrients();
}

bool Plant::isReadyToReproduce() const {
    return nutrients > spreadingThreshold;
}

void Plant::consumeResources() { 
    // Plants don't consume resources like animals do
    // They only lose nutrients when reproducing
}

Organism* Plant::reproduce() {
    if (!isReadyToReproduce()) {
        return nullptr;
    }
    
    consumeNutrients(spreadingThreshold / 2);
    
    float nutrientVariation = 0.8f + static_cast<float>(rand()) / RAND_MAX * 0.4f; // 0.8 to 1.2
    float growthVariation = 0.9f + static_cast<float>(rand()) / RAND_MAX * 0.2f;   // 0.9 to 1.1
    
    return new Plant(
        spreadingThreshold / 3,  
        maxLifespan,
        growthRate * growthVariation,
        nutrientAbsorptionRate * nutrientVariation
    );
}

void Plant::absorbNutrients() {
    float absorbed = nutrientAbsorptionRate * growthRate * 2.0f;
    addNutrients(absorbed);
    
    std::cout << "Plant absorbed " << absorbed << " nutrients, total: " << nutrients << std::endl;
}

void Plant::trySpread(Grid& grid, WorldManager& worldManager) {
    if (!isReadyToReproduce()) {
        std::cout << "Plant not ready to reproduce (nutrients: " << nutrients << "/" << spreadingThreshold << ")" << std::endl;
        return;
    }
    
    if (!position) {
        std::cout << "Plant has no position!" << std::endl;
        return;
    }
    
    std::vector<Position> adjacentPositions = position->getAdjacentPositions();
    std::vector<Position> validPositions;
    
    std::cout << "Checking " << adjacentPositions.size() << " adjacent positions for spreading..." << std::endl;
    
    for (const auto& pos : adjacentPositions) {
        if (grid.isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid.getTile(pos.getX(), pos.getY());
            if (tile.isEmpty()) {
                validPositions.push_back(pos);
                std::cout << "Found valid position: (" << pos.getX() << ", " << pos.getY() << ")" << std::endl;
            } else {
                std::cout << "Position (" << pos.getX() << ", " << pos.getY() << ") is occupied" << std::endl;
            }
        } else {
            std::cout << "Position (" << pos.getX() << ", " << pos.getY() << ") is out of bounds" << std::endl;
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
            std::cout << "Plant successfully spread to (" << spreadPos.getX() << ", " << spreadPos.getY() << ")" << std::endl;
        } else {
            std::cout << "Failed to create offspring!" << std::endl;
        }
    } else {
        std::cout << "No valid positions found for spreading" << std::endl;
    }
}