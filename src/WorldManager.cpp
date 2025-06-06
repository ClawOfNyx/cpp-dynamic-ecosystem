#include "WorldManager.h"
#include "WorldManagerImpl.h"

using namespace std;

WorldManager* WorldManager::instance = nullptr;

WorldManager::WorldManager(int width, int height, float baseNutrients) {
    pImpl = new WorldManagerImpl(width, height, baseNutrients);
}

WorldManager::~WorldManager() {
    delete pImpl;
}

WorldManager& WorldManager::getInstance(int width, int height, float baseNutrients){
    if (!instance) {
        instance = new WorldManager(width, height, baseNutrients);
    }
    return *instance;
}

void WorldManager::update() {
    pImpl->update(*this);
}

void WorldManager::addOrganism(Organism* organism, Position position) {
    pImpl->addOrganism(organism, position.getX(), position.getY());
}

void WorldManager::addOrganism(Organism* organism, int x, int y) {
    pImpl->addOrganism(organism, x, y);
}

void WorldManager::removeOrganism(Organism* organism) {
    pImpl->removeOrganism(organism);
}

void WorldManager::removeOrganism(Position position) {
    pImpl->removeOrganism(position.getX(), position.getY());
}

void WorldManager::spawnPlantFromDeadOrganism(Position position, float nutrients) {
    pImpl->spawnPlantFromDeadOrganism(position.getX(), position.getY(), nutrients);
}

const Grid& WorldManager::getGrid() const {
    return pImpl->getGrid();
}
int WorldManager::getOrganismCount() const {
    return pImpl->getOrganismCount();
}