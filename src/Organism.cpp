#include "Organism.h"

Organism::Organism(OrganismType type, float nutrients, int maxLifespan)
    : type(type), nutrients(nutrients), age(0), maxLifespan(maxLifespan) {}

OrganismType Organism::getType() const {
    return type;
}

float Organism::getNutrients() const {
    return nutrients;
}

void Organism::addNutrients(float amount) {
    nutrients += amount;
}

void Organism::consumeNutrients(float amount) {
    nutrients -= amount;
    if (nutrients < 0) nutrients = 0;
}

int Organism::getAge() const {
    return age;
}

void Organism::incrementAge() {
    ++age;
}

bool Organism::isReadyToReproduce() const {
    return nutrients > 10.0f;
}

bool Organism::isDead() const {
    return age >= maxLifespan || nutrients <= 0;
}
