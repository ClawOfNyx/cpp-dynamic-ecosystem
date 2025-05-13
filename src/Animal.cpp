#include "Animal.h"
#include <cstdlib>
#include <ctime>

Animal::Animal(float nutrients, 
               int maxLifespan, 
               int movementSpeed, 
               int visionDistance, 
               AnimalType animalType,
               float nutrientRequirement,
               float reproductionNutrientThreshold,
               int mass)
    : Organism(OrganismType::ANIMAL, nutrients, maxLifespan),
      movementSpeed(movementSpeed),
      visionDistance(visionDistance),
      animalType(animalType),
      nutrientRequirement(nutrientRequirement),
      reproductionNutrientThreshold(reproductionNutrientThreshold),
      mass(mass) {}

// Getters
int Animal::getMovementSpeed() const {
    return movementSpeed;
}

int Animal::getVisionDistance() const {
    return visionDistance;
}

AnimalType Animal::getAnimalType() const {
    return animalType;
}

float Animal::getNutrientRequirement() const {
    return nutrientRequirement;
}

float Animal::getReproductionNutrientThreshold() const {
    return reproductionNutrientThreshold;
}

int Animal::getMass() const {
    return mass;
}

// Setters
void Animal::setMovementSpeed(int speed) {
    movementSpeed = speed;
}

void Animal::setVisionDistance(int distance) {
    visionDistance = distance;
}

void Animal::setAnimalType(AnimalType type) {
    animalType = type;
}

void Animal::setNutrientRequirement(float requirement) {
    nutrientRequirement = requirement;
}

void Animal::setReproductionNutrientThreshold(float threshold) {
    reproductionNutrientThreshold = threshold;
}

void Animal::setMass(int newMass) {
    mass = newMass;
}

void Animal::update() {
    consumeResources();
    incrementAge();
}

bool Animal::isReadyToReproduce() const {
    return nutrients > reproductionNutrientThreshold;
}

void Animal::consumeResources() {
    consumeNutrients(nutrientRequirement);
}

Organism* Animal::reproduce() {
    if (!isReadyToReproduce()) {
        return nullptr;
    }

    consumeNutrients(reproductionNutrientThreshold / 2);

    float nutrientVariation = 0.9f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
    int speedVariation = static_cast<int>(0.9f + static_cast<float>(rand()) / RAND_MAX * 0.2f);
    
    return new Animal(
        reproductionNutrientThreshold / 2, 
        maxLifespan,                       
        static_cast<int>(movementSpeed * speedVariation),
        visionDistance,
        animalType,
        nutrientRequirement * nutrientVariation,
        reproductionNutrientThreshold,
        mass
    );
}

bool Animal::canEat(const Organism* food) const {
    if (food == nullptr) return false;
    
    if (animalType == AnimalType::HERBIVORE && food->getType() == OrganismType::PLANT) {
        return true;
    } else if (animalType == AnimalType::CARNIVORE && food->getType() == OrganismType::ANIMAL) {
        return true;
    } else if (animalType == AnimalType::OMNIVORE) {
        return true;
    }
    
    return false;
}

void Animal::eat(Organism* food) {
    if (food == nullptr || !canEat(food)) return;
    
    float foodNutrients = food->getNutrients();
    addNutrients(foodNutrients);
    
}