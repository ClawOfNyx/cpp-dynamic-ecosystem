#include "Plant.h"
#include "Grid.h"
#include <random>
#include <vector>

Plant::Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate)
    : Organism(OrganismType::PLANT, nutrients, maxLifespan),
      growthRate(growthRate), 
      nutrientAbsorptionRate(nutrientAbsorptionRate),
      spreadingThreshold(15.0f) {}

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

void Plant::update(Grid& grid) {
    absorbNutrients();
    incrementAge();
    
    // Try to spread if ready to reproduce
    if (isReadyToReproduce()) {
        trySpread(grid);
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
        nutrientAbsorptionRate
    );
}

void Plant::absorbNutrients() {
    addNutrients(nutrientAbsorptionRate * growthRate);
}

void Plant::trySpread(Grid& grid) {
    // Get adjacent positions
    std::vector<Position> adjacentPositions = position->getAdjacentPositions();
    std::vector<Position> validPositions;
    
    // Filter for valid, empty positions
    for (const auto& pos : adjacentPositions) {
        if (grid.isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid.getTile(pos.getX(), pos.getY());
            if (tile.isEmpty()) {
                validPositions.push_back(pos);
            }
        }
    }
    
    // If we have valid positions, create offspring
    if (!validPositions.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, validPositions.size() - 1);
        
        Position spreadPos = validPositions[dis(gen)];
        Plant* offspring = dynamic_cast<Plant*>(reproduce());
        
        if (offspring) {
            offspring->setPosition(spreadPos);
            Tile& targetTile = grid.getTile(spreadPos.getX(), spreadPos.getY());
            targetTile.setOccupant(*offspring);
            
            // Note: This creates offspring but doesn't add to organism list
            // WorldManager needs to handle this during its update cycle
        }
    }
}
