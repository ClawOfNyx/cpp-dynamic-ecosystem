#ifndef PLANT_H
#define PLANT_H

#include "Organism.h"
// #include "Grid.h"

class Plant : public Organism {
private:
    float growthRate;
    float nutrientAbsorptionRate;

public:
    Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate);
    
    // void trySpread(Grid& grid);
    // void update(Grid& grid) override;
};

#endif
