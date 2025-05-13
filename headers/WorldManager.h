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
    WorldManagerImpl* pImpl;
public:
    WorldManager(int width, int height, float baseNutrients = 1.0f);
    ~WorldManager();
    
    void update();

    void addOrganism(Organism* organism, Position position);
    void removeOrganism(Organism* organism);
    void removeOrganism(Position position);
    void spawnPlantFromDeadOrganism(Position position, float nutrients);
    
    Position findFood(const Animal* animal, Position position);
    bool moveAnimal(Animal* animal, Position fromPos, Position toPos);
    bool tryEating(Animal* animal, Position position);
    bool tryReproduction(Organism* organism, Position position);
    
    bool tryPlantSpread(Plant* plant, Position position);
    
    Position findClosestOrganismOfType(Position position, OrganismType type);
};

#endif