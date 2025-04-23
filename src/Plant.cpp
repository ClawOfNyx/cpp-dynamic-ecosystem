#include "Plant.h"

Plant::Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate)
    : Organism(OrganismType::PLANT, nutrients, maxLifespan),
      growthRate(growthRate), nutrientAbsorptionRate(nutrientAbsorptionRate) {}

// void Plant::trySpread(Grid& grid) {
//     // Implementation for plant spreading
// }

// void Plant::update(Grid& grid) {
//     nutrients += nutrientAbsorptionRate;
//     incrementAge();
//     // Possibly try to spread
//     trySpread(grid);
// }
