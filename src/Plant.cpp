#include "Plant.h"

Plant::Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate)
    : Organism(OrganismType::PLANT, nutrients, maxLifespan),
      growthRate(growthRate), 
      nutrientAbsorptionRate(nutrientAbsorptionRate),
      spreadingThreshold(15.0f) {}

// ------------------- Getters -------------------
float Plant::getGrowthRate() const {
    return growthRate;
}

float Plant::getNutrientAbsorptionRate() const {
    return nutrientAbsorptionRate;
}

// ------------------- Setters -------------------
void Plant::setGrowthRate(float rate) {
    growthRate = rate;
}

void Plant::setNutrientAbsorptionRate(float rate) {
    nutrientAbsorptionRate = rate;
}

void Plant::update() {
    absorbNutrients();
    incrementAge();
}

bool Plant::isReadyToReproduce() const {
    return nutrients > spreadingThreshold;
}

void Plant::consumeResources() { }

Organism* Plant::reproduce() {
    if (!isReadyToReproduce()) {
        return nullptr;
    }
    
    consumeNutrients(spreadingThreshold / 2);
    

    return new Plant(
        spreadingThreshold / 2,  // Start with half the threshold nutrients
        maxLifespan,
        growthRate,
        nutrientAbsorptionRate
    );
}

void Plant::absorbNutrients() {
    addNutrients(nutrientAbsorptionRate * growthRate);
}