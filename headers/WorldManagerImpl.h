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
public:
    WorldManagerImpl(int width, int height, float nutrients);
    ~WorldManagerImpl();

    void update(WorldManager& worldManager);

    void addOrganism(Organism* organism, int x, int y);
    void removeOrganism(Organism* organism);
    void removeOrganism(int x, int y);
    void spawnPlantFromDeadOrganism(int x, int y, float nutrients, float globalSpreadingThreshold);
    const Grid& getGrid() const;
    int getOrganismCount() const;
    void removeDeadOrganisms(float spreadingThreshold);
};

#endif