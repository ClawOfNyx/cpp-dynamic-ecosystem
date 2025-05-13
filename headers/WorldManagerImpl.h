#ifndef WORLD_MANAGER_IMPL_H
#define WORLD_MANAGER_IMPL_H
#include <unordered_map>
#include <vector>
#include <random>
#include "Grid.h"
#include "Organism.h"
#include "Animal.h"
#include "Plant.h"
#include "Position.h"

class WorldManagerImpl {
private:
    Grid* grid;
    std::vector<Organism*> organisms;
    float baseNutrientGenerationRate;
    std::unordered_map<Organism*, std::pair<int, int>> organismPositions;
    std::mt19937 rng;
    bool isPositionEmpty(int x, int y) const;
    std::pair<int, int> getRandomAdjacentPosition(int x, int y);
public:
    WorldManagerImpl(int width, int height, float nutrients);
    ~WorldManagerImpl();

    void update();

    void addOrganism(Organism* organism, int x, int y);
    void removeOrganism(Organism* organism);
    void removeOrganism(int x, int y);
    void spawnPlantFromDeadOrganism(int x, int y, float nutrients);
    
    std::pair<int, int> findFood(const Animal* animal, int x, int y);
    bool moveAnimal(Animal* animal, int fromX, int fromY, int toX, int toY);
    bool tryEating(Animal* animal, int x, int y);
    bool tryReproduction(Organism* organism, int x, int y);
    
    bool tryPlantSpread(Plant* plant, int x, int y);
    
    std::pair<int, int> findClosestOrganismOfType(int x, int y, OrganismType type);
};

#endif