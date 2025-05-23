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
    WorldManager(int width, int height, float baseNutrients = 1.0f);
    ~WorldManager();
public:
    
    static WorldManager& getInstance(int width, int height, float nutrient);
    
    void update();

    void addOrganism(Organism* organism, Position position);
    void removeOrganism(Organism* organism);
    void removeOrganism(Position position);
    void spawnPlantFromDeadOrganism(Position position, float nutrients);

    WorldManager(const WorldManager&) = delete;
    WorldManager& operator=(const WorldManager&) = delete;
};

#endif