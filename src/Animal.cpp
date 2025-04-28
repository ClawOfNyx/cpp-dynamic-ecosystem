#include "Animal.h"
#include "position.h"
// #include "Grid.h" // Include the Grid header for the implementation

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

// Position Animal::findFood(Grid& grid) {
    
//     return Position(0, 0);
// }

// void Animal::moveTowardsPosition(Position targetPos) {
    
// }

// bool Animal::eat(Organism* food) {
//     if (food == nullptr) return false;
    
//     bool canEat = false;
    
//     // Check if animal can eat the food based on its type
//     if (animalType == AnimalType::HERBIVORE && food->getType() == OrganismType::PLANT) {
//         canEat = true;
//     } else if (animalType == AnimalType::CARNIVORE && food->getType() == OrganismType::ANIMAL) {
//         canEat = true;
//     } else if (animalType == AnimalType::OMNIVORE) {
//         canEat = true;
//     }
    
//     if (canEat) {
//         float foodNutrients = food->getNutrients();
//         food->consumeNutrients(foodNutrients);
//         addNutrients(foodNutrients);
//         return true;
//     }
    
//     return false;
// }

// bool Animal::tryReproduce(Grid& grid) {
//     // Check if animal has enough nutrients to reproduce
//     if (getNutrients() > reproductionNutrientThreshold) {
//         consumeNutrients(reproductionNutrientThreshold / 2);
//         return true;
//     }
//     return false;
// }

// void Animal::update(Grid& grid) {
//     consumeNutrients(nutrientRequirement);
//     incrementAge();
//     if (getNutrients() < reproductionNutrientThreshold) {
//         Position foodPos = findFood(grid);
//         moveTowardsPosition(foodPos);
//     }
//     tryReproduce(grid);
// }
