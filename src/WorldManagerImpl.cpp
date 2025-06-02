#include "WorldManagerImpl.h"
#include "WorldManager.h"
#include <algorithm>
#include <iostream>

WorldManagerImpl::WorldManagerImpl(int width, int height, float nutrients)
    : baseNutrientGenerationRate(nutrients) {
    grid = new Grid(width, height);
}

WorldManagerImpl::~WorldManagerImpl() {
    // Clean up all organisms
    for (Organism* organism : organisms) {
        delete organism;
    }
    organisms.clear();
    
    delete grid;
}

void WorldManagerImpl::update(WorldManager& worldManager) {
    std::cout << "Updating " << organisms.size() << " organisms" << std::endl;
    
    std::vector<bool> shouldUpdate(organisms.size(), true);
    
    for (size_t i = 0; i < organisms.size() && i < shouldUpdate.size(); ++i) {
        if (shouldUpdate[i] && organisms[i] != nullptr && !organisms[i]->isDead()) {
            // Check if organism is still at the same index (not moved due to removals)
            if (i < organisms.size() && organisms[i] != nullptr) {
                organisms[i]->update(*grid, worldManager);
            }
        }
    }
    
    removeDeadOrganisms();
}

void WorldManagerImpl::addOrganism(Organism* organism, int x, int y) {
    if (!organism) {
        std::cout << "Warning: Attempted to add null organism" << std::endl;
        return;
    }
    
    if (!grid->isInBounds(x, y)) {
        std::cout << "Warning: Attempted to add organism out of bounds at (" << x << ", " << y << ")" << std::endl;
        delete organism; // Clean up the organism since we can't place it
        return;
    }
    
    Tile& tile = grid->getTile(x, y);
    if (!tile.isEmpty()) {
        std::cout << "Cannot add organism - tile occupied at (" << x << ", " << y << ")" << std::endl;
        delete organism; // Clean up the organism since we can't place it
        return;
    }
    
    // Only set position and add to organisms vector if we can successfully place it
    Position pos(x, y);
    organism->setPosition(pos);
    
    try {
        tile.setOccupant(*organism);
        organisms.push_back(organism); // Only add to vector if tile placement succeeds
        std::cout << "Added organism to (" << x << ", " << y << ")" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Failed to place organism: " << e.what() << std::endl;
        delete organism; // Clean up if placement fails
    }
}

void WorldManagerImpl::removeOrganism(Organism* organism) {
    if (!organism) return;
    
    // Find and remove from organisms vector first
    auto it = std::find(organisms.begin(), organisms.end(), organism);
    if (it != organisms.end()) {
        organisms.erase(it);
        
        // Clear from grid
        Position pos = organism->getPosition();
        if (grid->isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid->getTile(pos.getX(), pos.getY());
            if (!tile.isEmpty() && tile.getOccupant() == organism) {
                tile.clearOccupant();
            }
        }
        
        delete organism;
    }
}

void WorldManagerImpl::removeOrganism(int x, int y) {
    if (grid->isInBounds(x, y)) {
        Tile& tile = grid->getTile(x, y);
        if (!tile.isEmpty()) {
            Organism* organism = tile.getOccupant();
            removeOrganism(organism);
        }
    }
}

void WorldManagerImpl::spawnPlantFromDeadOrganism(int x, int y, float nutrients) {
    if (grid->isInBounds(x, y)) {
        Tile& tile = grid->getTile(x, y);
        if (tile.isEmpty()) {
            float plantNutrients = std::max(nutrients, 6.0f); 
            Plant* newPlant = new Plant(plantNutrients, 100, 0.8f, 0.6f);
            addOrganism(newPlant, x, y);
            std::cout << "Plant spawned from dead organism with " << plantNutrients << " nutrients" << std::endl;
        }
    }
}

const Grid& WorldManagerImpl::getGrid() const {
    return *grid;
}

int WorldManagerImpl::getOrganismCount() const {
    return organisms.size();
}

void WorldManagerImpl::removeDeadOrganisms() {
    std::vector<std::pair<Position, float>> plantsToSpawn;
    
    auto it = organisms.begin();
    while (it != organisms.end()) {
        Organism* organism = *it;
        if (!organism || organism->isDead()) {
            if (organism) {
                Position pos = organism->getPosition();
                float nutrients = organism->getNutrients();
                
                std::cout << "Organism died at (" << pos.getX() << ", " << pos.getY() 
                         << ") with " << nutrients << " nutrients" << std::endl;
                
                // Give more nutrients to spawned plants and ensure minimum threshold
                float plantNutrients = std::max(nutrients * 0.8f, 12.0f); // 80% of nutrients, minimum 12
                plantsToSpawn.emplace_back(pos, plantNutrients);
                
                // Clear from grid
                if (grid->isInBounds(pos.getX(), pos.getY())) {
                    Tile& tile = grid->getTile(pos.getX(), pos.getY());
                    if (!tile.isEmpty() && tile.getOccupant() == organism) {
                        tile.clearOccupant();
                    }
                }
                
                delete organism;
            }
            
            it = organisms.erase(it);
        } else {
            ++it;
        }
    }
    
    // Spawn plants from dead organisms
    for (const auto& plantData : plantsToSpawn) {
        const Position& pos = plantData.first;
        float nutrients = plantData.second;
        
        if (grid->isInBounds(pos.getX(), pos.getY())) {
            Tile& tile = grid->getTile(pos.getX(), pos.getY());
            if (tile.isEmpty()) {
                // Create plants with better stats specifically for decomposition plants
                Plant* newPlant = new Plant(
                    nutrients,           // Use the calculated nutrients (minimum 12)
                    120,                // Longer lifespan 
                    1.0f,               // Higher growth rate
                    0.8f                // Higher absorption rate
                );
                addOrganism(newPlant, pos.getX(), pos.getY());
                std::cout << "Decomposition plant spawned at (" << pos.getX() << ", " << pos.getY() 
                         << ") with " << nutrients << " nutrients" << std::endl;
            }
        }
    }
}