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
    
    // Pass the global spreading threshold from the WorldManager
    removeDeadOrganisms(worldManager.getGlobalSpreadingThreshold());
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

void WorldManagerImpl::spawnPlantFromDeadOrganism(int x, int y, float nutrients, float globalSpreadingThreshold) {
    // For animals (which have higher nutrients), always try to spawn a plant
    // For plants that die, use the randomization to prevent infinite spawn/die loops
    bool isFromAnimal = nutrients > 5.0f;
    
    // If it's a plant with low nutrients, only spawn with 50% probability
    if (!isFromAnimal && (rand() % 100) < 50) {
        return; // Skip plant spawning for low-nutrient plants
    }
    
    if (grid->isInBounds(x, y)) {
        Tile& tile = grid->getTile(x, y);
        if (tile.isEmpty()) {
            // Create a plant with randomized properties
            float growthRate = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.5f; // 0.3 to 0.8
            float absorptionRate = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f; // 0.2 to 0.5
            
            // Ensure enough nutrients for survival
            float startingNutrients = std::max(2.0f, nutrients);
            
            // Calculate spreading threshold based on growth rate and global threshold
            float plantSpreadingThreshold = globalSpreadingThreshold * (1.0f + growthRate);
            
            // Create the new plant with the calculated spreading threshold
            Plant* newPlant = new Plant(startingNutrients, 100, growthRate, absorptionRate, plantSpreadingThreshold);
            addOrganism(newPlant, x, y);
        }
    }
}

const Grid& WorldManagerImpl::getGrid() const {
    return *grid;
}

int WorldManagerImpl::getOrganismCount() const {
    return organisms.size();
}

void WorldManagerImpl::removeDeadOrganisms(float spreadingThreshold) {
    
    auto it = organisms.begin();
    while (it != organisms.end()) {
        Organism* organism = *it;
        
        if (organism->isDead()) {
            // Get position before removing
            Position pos = organism->getPosition();
            int x = pos.getX();
            int y = pos.getY();
            float nutrients = organism->getNutrients();
            
            // Clear the tile
            if (grid->isInBounds(x, y)) {
                Tile& tile = grid->getTile(x, y);
                if (tile.getOccupant() == organism) {
                    tile.clearOccupant();
                }
            }
            
            // Remove from vector
            it = organisms.erase(it);
            
            // Attempt to spawn a plant at this location with spreading threshold
            spawnPlantFromDeadOrganism(x, y, nutrients, spreadingThreshold);
            
            // Delete the organism
            delete organism;
        } else {
            ++it;
        }
    }
}