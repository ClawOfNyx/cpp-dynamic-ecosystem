#include "Organism.h"

Organism::Organism(OrganismType type, float nutrients, int maxLifespan)
    : type(type), nutrients(nutrients), age(0), maxLifespan(maxLifespan), position(nullptr) { }

Organism::~Organism() {
    delete position;
}

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

bool Organism::isDead() const {
    return age >= maxLifespan || nutrients <= 0;
}

const Position& Organism::getPosition() const {
    return *position;
}

void Organism::setPosition(const Position& newPos) {
    if (position == nullptr) {
        // Create new position if it doesn't exist
        position = new Position(newPos.getX(), newPos.getY());
    } else {
        // Update existing position
        position->setX(newPos.getX());
        position->setY(newPos.getY());
    }
}

Organism* Organism::reproduce() {
    // Default implementation returns nullptr
    // Derived classes will override with specific reproduction logic
    return nullptr;
}