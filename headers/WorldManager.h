#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H
#include <vector>
#include "Grid.h"
#include "Organism.h"
#include "Animal.h"
#include "Plant.h"
#include "Position.h"

class WorldManagerImpl;

class WorldManager {
private:
    static WorldManager* instance;

    WorldManagerImpl* pImpl;
    float globalSpreadingThreshold = 12.0f; // Default value
    WorldManager(int width, int height, float baseNutrients = 1.0f);
    ~WorldManager();
public:
    
    static WorldManager& getInstance(int width, int height, float nutrient);
    
    void update();

    void addOrganism(Organism* organism, Position position);
    void addOrganism(Organism* organism, int x, int y);
    void removeOrganism(Organism* organism);
    void removeOrganism(Position position);
    void spawnPlantFromDeadOrganism(Position position, float nutrients, float spreadingThreshold);
    const Grid& getGrid() const;
    int getOrganismCount() const;
    
    // Getter and setter for global spreading threshold
    float getGlobalSpreadingThreshold() const { return globalSpreadingThreshold; }
    void setGlobalSpreadingThreshold(float threshold) { globalSpreadingThreshold = threshold; }

    WorldManager(const WorldManager&) = delete;
    WorldManager& operator=(const WorldManager&) = delete;
};

#endif