#ifndef PLANT_H
#define PLANT_H

#include "Organism.h"

class Plant : public Organism {
private:
    float growthRate;
    float nutrientAbsorptionRate;
    float spreadingThreshold;

public:
    Plant(float nutrients, int maxLifespan, float growthRate, float nutrientAbsorptionRate);
    
    // Getters
    float getGrowthRate() const;
    float getNutrientAbsorptionRate() const;
    
    // Setters
    void setGrowthRate(float rate);
    void setNutrientAbsorptionRate(float rate);
    
    void update() override;
    bool isReadyToReproduce() const override;
    void consumeResources() override;
    
    Organism* reproduce() override;

    void absorbNutrients();
};

#endif
