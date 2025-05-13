#include "WorldManager.h"
#include "WorldManagerImpl.h"

using namespace std;

WorldManager::WorldManager(int width, int height, float baseNutrients) {
    pImpl = new WorldManagerImpl(width, height, baseNutrients);
}

WorldManager::~WorldManager() {
    delete pImpl;
}

void WorldManager::update() {
    pImpl->update();
}

void WorldManager::addOrganism(Organism* organism, Position position) {
    pImpl->addOrganism(organism, position.getX(), position.getY());
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

Position WorldManager::findFood(const Animal* animal, Position position) {
    auto result = pImpl->findFood(animal, position.getX(), position.getY());
    return Position(result.first, result.second);
}

bool WorldManager::moveAnimal(Animal* animal, Position fromPos, Position toPos) {
    return pImpl->moveAnimal(animal, fromPos.getX(), fromPos.getY(), toPos.getX(), toPos.getY());
}

bool WorldManager::tryEating(Animal* animal, Position position) {
    return pImpl->tryEating(animal, position.getX(), position.getY());
}

bool WorldManager::tryReproduction(Organism* organism, Position position) {
    return pImpl->tryReproduction(organism, position.getX(), position.getY());
}

bool WorldManager::tryPlantSpread(Plant* plant, Position position) {
    return pImpl->tryPlantSpread(plant, position.getX(), position.getY());
}

Position WorldManager::findClosestOrganismOfType(Position position, OrganismType type) {
    auto result = pImpl->findClosestOrganismOfType(position.getX(), position.getY(), type);
    return Position(result.first, result.second);
}
