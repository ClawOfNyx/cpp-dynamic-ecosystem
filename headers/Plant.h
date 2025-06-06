#ifndef PLANT_H
#define PLANT_H

#include "Organism.h"

class Grid;

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
    
    void update(Grid& grid, WorldManager& worldManager) override;
    bool isReadyToReproduce() const override;
    void consumeResources() override;
    
    Organism* reproduce() override;

    void absorbNutrients();
    void trySpread(Grid& grid, WorldManager& worldManager);
};

#endif
