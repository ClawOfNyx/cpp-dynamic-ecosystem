#include "WorldManagerImpl.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <iostream>

using namespace std;

WorldManagerImpl::WorldManagerImpl(int width, int height, float nutrients) 
    : grid(nullptr),
      organisms(),
      baseNutrientGenerationRate(nutrients) {
    
    cout << "Creating Grid with dimensions: " << width << "x" << height << "..." << endl;
    grid = new Grid(width, height);
    cout << "Grid created successfully" << endl;
    
    cout << "Grid initialization completed" << endl;

}

WorldManagerImpl::~WorldManagerImpl() {
    for (auto organism : organisms) {
        delete organism;
    }
    organisms.clear();

    delete grid;
}

void WorldManagerImpl::update() {
    for (auto* organism : organisms) {
        if (organism->getType() == OrganismType::PLANT) {
            organism->update();
        }
    }

    for (auto* organism : organisms) {
        if (auto* animal = dynamic_cast<Animal*>(organism)) {
            if (animal->getAnimalType() == AnimalType::HERBIVORE) {
                organism->update();
            }
        }
    }

    for (auto* organism : organisms) {
        if (auto* animal = dynamic_cast<Animal*>(organism)) {
            if (animal->getAnimalType() == AnimalType::CARNIVORE) {
                organism->update();
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
    
    organisms.push_back(organism);
}

void WorldManagerImpl::removeOrganism(Organism* organism) {
    //Tile& tile = organism->getTile();
    //tile.clearOccupant();
    delete organism; 
}

void WorldManagerImpl::removeOrganism(int x, int y) {
    Tile& tile = grid->getTile(x,y);
    Organism* organism = tile.getOccupant();
    tile.clearOccupant();
    delete organism;
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
