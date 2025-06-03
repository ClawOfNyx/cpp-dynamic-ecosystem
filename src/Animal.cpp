#include "Animal.h"
#include "Grid.h"
#include "Plant.h"
#include "WorldManager.h"  // ADD THIS LINE
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

Animal::Animal(float nutrients, 
               int maxLifespan, 
               int movementSpeed, 
               int visionDistance, 
               AnimalType animalType,
               float nutrientRequirement,
               float reproductionNutrientThreshold,
               int mass)
    : Organism(OrganismType::ANIMAL, nutrients, maxLifespan),
      movementSpeed(movementSpeed),
      visionDistance(visionDistance),
      animalType(animalType),
      nutrientRequirement(nutrientRequirement),
      reproductionNutrientThreshold(reproductionNutrientThreshold),
      mass(mass) {}

// Getters and Setters remain the same...
int Animal::getMovementSpeed() const { return movementSpeed; }
int Animal::getVisionDistance() const { return visionDistance; }
AnimalType Animal::getAnimalType() const { return animalType; }
float Animal::getNutrientRequirement() const { return nutrientRequirement; }
float Animal::getReproductionNutrientThreshold() const { return reproductionNutrientThreshold; }
int Animal::getMass() const { return mass; }

void Animal::setMovementSpeed(int speed) { movementSpeed = speed; }
void Animal::setVisionDistance(int distance) { visionDistance = distance; }
void Animal::setAnimalType(AnimalType type) { animalType = type; }
void Animal::setNutrientRequirement(float requirement) { nutrientRequirement = requirement; }
void Animal::setReproductionNutrientThreshold(float threshold) { reproductionNutrientThreshold = threshold; }
void Animal::setMass(int newMass) { mass = newMass; }

void Animal::update(Grid& grid, WorldManager& worldManager) {
    consumeResources();
    incrementAge();
    
    hunt(grid, worldManager);     
    move(grid);           
    
    if (isReadyToReproduce()) {
        tryReproduce(grid, worldManager);
    }
}

bool Animal::isReadyToReproduce() const {
    return nutrients > reproductionNutrientThreshold;
}

void Animal::consumeResources() {
    consumeNutrients(nutrientRequirement);
}

Organism* Animal::reproduce() {
    if (!isReadyToReproduce()) {
        return nullptr;
    }

    consumeNutrients(reproductionNutrientThreshold / 2);

    float nutrientVariation = 0.9f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
    int speedVariation = static_cast<int>(0.9f + static_cast<float>(rand()) / RAND_MAX * 0.2f);
    
    return new Animal(
        reproductionNutrientThreshold / 2, 
        maxLifespan,                       
        static_cast<int>(movementSpeed * speedVariation),
        visionDistance,
        animalType,
        nutrientRequirement * nutrientVariation,
        reproductionNutrientThreshold,
        mass
    );
}

bool Animal::canEat(const Organism* food) const {
    if (food == nullptr) return false;
    
    if (animalType == AnimalType::HERBIVORE && food->getType() == OrganismType::PLANT) {
        return true;
    } else if (animalType == AnimalType::CARNIVORE && food->getType() == OrganismType::ANIMAL) {
        // Carnivores should only eat herbivores, not other carnivores
        const Animal* animalFood = dynamic_cast<const Animal*>(food);
        if (animalFood && animalFood->getAnimalType() == AnimalType::HERBIVORE) {
            return true;
        }
        return false;
    } else if (animalType == AnimalType::OMNIVORE) {
        return true;
    }
    
    return false;
}

void Animal::eat(Organism* food) {
    if (food == nullptr || !canEat(food)) return;
    
    float foodNutrients = food->getNutrients();
    addNutrients(foodNutrients);
}

void Animal::move(Grid& grid) {
    Position newPos = findBestMovePosition(grid);
    
    // Clear current tile
    Tile& currentTile = grid.getTile(position->getX(), position->getY());
    currentTile.clearOccupant();
    
    // Move to new position
    setPosition(newPos);
    
    // Occupy new tile
    Tile& newTile = grid.getTile(newPos.getX(), newPos.getY());
    newTile.setOccupant(*this);
}

void Animal::hunt(Grid& grid, WorldManager& worldManager) {
    Organism* nearestFood = findNearestFood(grid);
    
    if (nearestFood) {
        int distance = position->distanceToPoint(nearestFood->getPosition());
        
        // If food is adjacent, eat it
        if (distance <= 1) {
            eat(nearestFood);
            
            // Remove eaten organism from grid and world manager
            Position foodPos = nearestFood->getPosition();
            worldManager.removeOrganism(foodPos);
        }
    }
}

Position Animal::findBestMovePosition(Grid& grid) {
    std::vector<Position> adjacentPositions = position->getAdjacentPositions();
    std::vector<Position> validPositions;
    
    // Filter for positions that are within bounds AND empty
    for (const auto& pos : adjacentPositions) {
        if (grid.isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid.getTile(pos.getX(), pos.getY());
            if (tile.isEmpty()) {
                validPositions.push_back(pos);
            }
        }
    }
    
    if (validPositions.empty()) {
        return *position; // Stay in place if no valid moves
    }
    
    // Try to move towards food
    Organism* nearestFood = findNearestFood(grid);
    if (nearestFood) {
        Position foodPos = nearestFood->getPosition();
        
        // Find the position that gets us closest to food
        Position bestPos = validPositions[0];
        int shortestDistance = bestPos.distanceToPoint(foodPos);
        
        for (const auto& pos : validPositions) {
            int distance = pos.distanceToPoint(foodPos);
            if (distance < shortestDistance) {
                shortestDistance = distance;
                bestPos = pos;
            }
        }
        
        return bestPos;
    }
    
    // Random movement if no food found
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validPositions.size() - 1);
    
    return validPositions[dis(gen)];
}

void Animal::tryReproduce(Grid& grid, WorldManager& worldManager) {
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
        
        Position birthPos = validPositions[dis(gen)];
        Animal* offspring = dynamic_cast<Animal*>(reproduce());
        
        if (offspring) {
            worldManager.addOrganism(offspring, birthPos);
            std::cout << "Animal reproduced at (" << birthPos.getX() << ", " << birthPos.getY() << ")" << std::endl;
        }
    }
}

Organism* Animal::findNearestFood(Grid& grid) {
    Organism* nearestFood = nullptr;
    int shortestDistance = visionDistance + 1;
    
    // Search within vision distance
    for (int dy = -visionDistance; dy <= visionDistance; ++dy) {
        for (int dx = -visionDistance; dx <= visionDistance; ++dx) {
            int checkX = position->getX() + dx;
            int checkY = position->getY() + dy;
            
            if (grid.isInBounds(checkX, checkY)) {
                Tile& tile = grid.getTile(checkX, checkY);
                if (!tile.isEmpty()) {
                    Organism* organism = tile.getOccupant();
                    if (canEat(organism)) {
                        int distance = position->distanceToPoint(organism->getPosition());
                        if (distance < shortestDistance) {
                            shortestDistance = distance;
                            nearestFood = organism;
                        }
                    }
                }
            }
        }
    }
    
    return nearestFood;
}