#ifndef ANIMAL_H
#define ANIMAL_H

#include "Organism.h"
// #include "Grid.h"

enum class AnimalType {
    HERBIVORE,
    CARNIVORE,
    OMNIVORE
};

class Animal : public Organism {
private:
    int movementSpeed;
    int visionDistance;
    AnimalType animalType;
    float nutrientRequirement;
    float reproductionNutrientThreshold;
    int mass;

public:
    Animal(float nutrients, 
           int maxLifespan, 
           int movementSpeed, 
           int visionDistance, 
           AnimalType animalType,
           float nutrientRequirement,
           float reproductionNutrientThreshold,
           int mass);

    // Getters
    int getMovementSpeed() const;
    int getVisionDistance() const;
    AnimalType getAnimalType() const;
    float getNutrientRequirement() const;
    float getReproductionNutrientThreshold() const;
    int getMass() const;

    // Setters
    void setMovementSpeed(int speed);
    void setVisionDistance(int distance);
    void setAnimalType(AnimalType type);
    void setNutrientRequirement(float requirement);
    void setReproductionNutrientThreshold(float threshold);
    void setMass(int newMass);

    // Behaviors
    // Position findFood(Grid& grid);
    // void moveTowardsPosition(Position targetPos);
    // bool eat(Organism* food);
    // bool tryReproduce(Grid& grid);
    // void update(Grid& grid);
};

#endif