#include "WorldManagerImpl.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

WorldManagerImpl::WorldManagerImpl(int width, int height, float nutrients) 
    : grid(new Grid(width, height)),
      organisms(),
      baseNutrientGenerationRate(nutrients),
      organismPositions(),
      rng(std::chrono::system_clock::now().time_since_epoch().count()) {
}

WorldManagerImpl::~WorldManagerImpl() {
    for (auto organism : organisms) {
        delete organism;
    }
    organisms.clear();
    organismPositions.clear();

    delete grid;
}

void WorldManagerImpl::update() {
    std::vector<Organism*> currentOrganisms = organisms;
    
    for (auto organism : currentOrganisms) {
        if (std::find(organisms.begin(), organisms.end(), organism) == organisms.end()) {
            continue;
        }
        
        auto posIt = organismPositions.find(organism);
        if (posIt == organismPositions.end()) {
            continue;
        }
        
        int x = posIt->second.first;
        int y = posIt->second.second;
        
        organism->incrementAge();
        
        if (organism->isDead()) {
            float remainingNutrients = organism->getNutrients() * 0.7f;
            removeOrganism(organism);
            spawnPlantFromDeadOrganism(x, y, remainingNutrients);
            continue;
        }
        
        organism->consumeResources();
        
        if (organism->getType() == OrganismType::ANIMAL) {
            Animal* animal = static_cast<Animal*>(organism);
            
            if (animal->getNutrients() < animal->getNutrientRequirement() * 2) {
                auto foodPos = findFood(animal, x, y);
                
                if (foodPos.first != -1 && foodPos.second != -1) {
                    int dx = foodPos.first - x;
                    int dy = foodPos.second - y;

                    if (dx != 0) dx = dx / abs(dx);
                    if (dy != 0) dy = dy / abs(dy);
                    
                    int newX = x + dx;
                    int newY = y + dy;
                    
                    if (grid->isInBounds(newX, newY) && isPositionEmpty(newX, newY)) {
                        moveAnimal(animal, x, y, newX, newY);
                        x = newX;
                        y = newY;
                    }
                    
                    if (abs(foodPos.first - x) <= 1 && abs(foodPos.second - y) <= 1) {
                        tryEating(animal, foodPos.first, foodPos.second);
                    }
                } else {
                    auto newPos = getRandomAdjacentPosition(x, y);
                    if (newPos.first != -1 && isPositionEmpty(newPos.first, newPos.second)) {
                        moveAnimal(animal, x, y, newPos.first, newPos.second);
                    }
                }
            } else if (animal->isReadyToReproduce()) {
                tryReproduction(animal, x, y);
            } else {
                auto newPos = getRandomAdjacentPosition(x, y);
                if (newPos.first != -1 && isPositionEmpty(newPos.first, newPos.second)) {
                    moveAnimal(animal, x, y, newPos.first, newPos.second);
                }
            }
        } else if (organism->getType() == OrganismType::PLANT) {
            Plant* plant = static_cast<Plant*>(organism);
            
            plant->update();
            
            if (plant->isReadyToReproduce()) {
                tryPlantSpread(plant, x, y);
            }
        }
    }
}

void WorldManagerImpl::addOrganism(Organism* organism, int x, int y) {
    if (!grid->isInBounds(x, y)) {
        throw std::out_of_range("Cannot add organism outside grid boundaries");
    }
    
    Tile& tile = grid->getTile(x, y);
    if (!tile.isEmpty()) {
        throw std::runtime_error("Cannot add organism to an occupied tile");
    }

    tile.setOccupant(*organism);

    organismPositions[organism] = std::make_pair(x, y);
    
    organisms.push_back(organism);
}

void WorldManagerImpl::removeOrganism(Organism* organism) {
    auto it = find(organisms.begin(), organisms.end(), organism);
    if (it != organisms.end()) {
        auto posIt = organismPositions.find(organism);
        if (posIt != organismPositions.end()) {
            int x = posIt->second.first;
            int y = posIt->second.second;
            
            if (grid->isInBounds(x, y)) {
                Tile& tile = grid->getTile(x, y);
                if (!tile.isEmpty() && &tile.getOccupant() == organism) {
                    tile.clearOccupant();
                }
            }
            
            organismPositions.erase(posIt);
        }
        
        organisms.erase(it);
        
        delete organism;
    }
}

void WorldManagerImpl::removeOrganism(int x, int y) {
    if (!grid->isInBounds(x, y)) {
        return;
    }

    Tile& tile = grid->getTile(x, y);
    if (!tile.isEmpty()) {
        Organism* organism = &tile.getOccupant();
        
        tile.clearOccupant();
        
        auto it = find(organisms.begin(), organisms.end(), organism);
        if (it != organisms.end()) {
            organisms.erase(it);
        }
        
        organismPositions.erase(organism);
        
        delete organism;
    }
}

void WorldManagerImpl::spawnPlantFromDeadOrganism(int x, int y, float nutrients) {
    if (!grid->isInBounds(x, y)) {
        return;
    }
    
    Tile& tile = grid->getTile(x, y);
    if (!tile.isEmpty()) {
        Position pos(x, y);
        Tile& emptyTile = grid->findClosestEmptyTile(pos);
        x = emptyTile.getPosition().getX();
        y = emptyTile.getPosition().getY();
    }
    
    float growthRateFactor = std::min(1.0f, nutrients / 20.0f);
    int plantLifespan = 100 + static_cast<int>(nutrients * 5);
    float growthRate = 0.5f + growthRateFactor;
    float absorptionRate = 0.3f + (growthRateFactor * 0.7f);
    
    Plant* newPlant = new Plant(
        nutrients,
        plantLifespan,
        growthRate,
        absorptionRate
    );
    
    addOrganism(newPlant, x, y);
}

std::pair<int, int> WorldManagerImpl::findFood(const Animal* animal, int x, int y) {
    AnimalType animalType = animal->getAnimalType();
    int visionDistance = animal->getVisionDistance();
    
    OrganismType targetType;
    switch (animalType) {
        case AnimalType::HERBIVORE:
            targetType = OrganismType::PLANT;
            break;
        case AnimalType::CARNIVORE:
            targetType = OrganismType::ANIMAL;
            break;
        case AnimalType::OMNIVORE:
            {
                auto animalFood = findClosestOrganismOfType(x, y, OrganismType::ANIMAL);
                if (animalFood.first != -1) {
                    return animalFood;
                }
                return findClosestOrganismOfType(x, y, OrganismType::PLANT);
            }
        default:
            return {-1, -1};
    }
    
    return findClosestOrganismOfType(x, y, targetType);
}

bool WorldManagerImpl::moveAnimal(Animal* animal, int fromX, int fromY, int toX, int toY) {
    if (!grid->isInBounds(fromX, fromY) || !grid->isInBounds(toX, toY)) {
        return false;
    }
    
    if (!isPositionEmpty(toX, toY)) {
        return false;
    }
    
    Tile& sourceTile = grid->getTile(fromX, fromY);
    if (sourceTile.isEmpty() || &sourceTile.getOccupant() != animal) {
        return false;
    }
    
    Tile& destTile = grid->getTile(toX, toY);
    
    sourceTile.clearOccupant();
    destTile.setOccupant(*animal);
    
    organismPositions[animal] = std::make_pair(toX, toY);
    
    float movementCost = 0.1f * animal->getMovementSpeed() * animal->getMass() / 10.0f;
    animal->consumeNutrients(movementCost);
    
    return true;
}

bool WorldManagerImpl::tryEating(Animal* animal, int x, int y) {
    if (!grid->isInBounds(x, y)) {
        return false;
    }
    
    Tile& tile = grid->getTile(x, y);
    if (tile.isEmpty()) {
        return false;
    }
    
    Organism* food = &tile.getOccupant();
    
    AnimalType animalType = animal->getAnimalType();
    OrganismType foodType = food->getType();
    
    bool canEat = false;
    
    switch (animalType) {
        case AnimalType::HERBIVORE:
            canEat = (foodType == OrganismType::PLANT);
            break;
        case AnimalType::CARNIVORE:
            canEat = (foodType == OrganismType::ANIMAL);
            break;
        case AnimalType::OMNIVORE:
            canEat = true;
            break;
        default:
            return false;
    }
    
    if (!canEat) {
        return false;
    }
    
    float nutrientsGained = food->getNutrients() * 0.8f;
    animal->addNutrients(nutrientsGained);
    
    removeOrganism(x, y);
    
    return true;
}

bool WorldManagerImpl::tryReproduction(Organism* organism, int x, int y) {
    if (!organism->isReadyToReproduce()) {
        return false;
    }
    
    Position position(x, y);
    std::vector<Position> adjacentPositions = position.getAdjacentPositions();
    std::vector<std::pair<int, int>> positionPairs;
    
    // Convert Position objects to pairs for shuffling
    for (const auto& pos : adjacentPositions) {
        positionPairs.push_back({pos.getX(), pos.getY()});
    }
    
    std::shuffle(positionPairs.begin(), positionPairs.end(), rng);
    
    for (const auto& pos : positionPairs) {
        if (isPositionEmpty(pos.first, pos.second)) {
            Organism* offspring = organism->reproduce();
            if (offspring) {
                addOrganism(offspring, pos.first, pos.second);
                return true;
            }
            return false;
        }
    }
    
    return false;
}

bool WorldManagerImpl::tryPlantSpread(Plant* plant, int x, int y) {
    if (!plant->isReadyToReproduce()) {
        return false;
    }
    
    std::vector<std::pair<int, int>> potentialPositions;
    
    for (int dx = -3; dx <= 3; dx++) {
        for (int dy = -3; dy <= 3; dy++) {
            if (dx == 0 && dy == 0) continue;
            
            int newX = x + dx;
            int newY = y + dy;
            
            if (grid->isInBounds(newX, newY) && isPositionEmpty(newX, newY)) {
                potentialPositions.push_back({newX, newY});
            }
        }
    }
    
    if (potentialPositions.empty()) {
        return false;
    }
    
    std::uniform_int_distribution<int> dist(0, potentialPositions.size() - 1);
    auto selectedPos = potentialPositions[dist(rng)];
    
    Organism* newPlant = plant->reproduce();
    if (newPlant) {
        addOrganism(newPlant, selectedPos.first, selectedPos.second);
        return true;
    }
    
    return false;
}


std::pair<int, int> WorldManagerImpl::findClosestOrganismOfType(int x, int y, OrganismType type) {
    const int MAX_SEARCH_DISTANCE = 10;
    
    for (int distance = 1; distance <= MAX_SEARCH_DISTANCE; distance++) {
        for (int dx = -distance; dx <= distance; dx++) {
            for (int dy = -distance; dy <= distance; dy++) {
                if (abs(dx) != distance && abs(dy) != distance) {
                    continue;
                }
                
                int newX = x + dx;
                int newY = y + dy;
                
                if (!grid->isInBounds(newX, newY)) {
                    continue;
                }
                
                Tile& tile = grid->getTile(newX, newY);
                if (!tile.isEmpty() && tile.getOccupant().getType() == type) {
                    return {newX, newY};
                }
            }
        }
    }
    
    return {-1, -1};
}

bool WorldManagerImpl::isPositionEmpty(int x, int y) const {
    if (!grid->isInBounds(x, y)) {
        return false;
    }
    
    return grid->getTile(x, y).isEmpty();
}

std::pair<int, int> WorldManagerImpl::getRandomAdjacentPosition(int x, int y) {
    Position position(x, y);
    std::vector<Position> adjacentPositions = position.getAdjacentPositions();
    std::vector<std::pair<int, int>> validPositions;
    
    // Filter positions that are in bounds
    for (const auto& pos : adjacentPositions) {
        int newX = pos.getX();
        int newY = pos.getY();
        if (grid->isInBounds(newX, newY)) {
            validPositions.push_back({newX, newY});
        }
    }
    
    if (validPositions.empty()) {
        return {-1, -1};
    }
    
    std::uniform_int_distribution<int> dist(0, validPositions.size() - 1);
    return validPositions[dist(rng)];
}


