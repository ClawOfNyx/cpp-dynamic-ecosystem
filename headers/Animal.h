#ifndef ANIMAL_H
#define ANIMAL_H

#include "Organism.h"

class Grid;

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

    void update(Grid& grid, WorldManager& worldManager) override;
    bool isReadyToReproduce() const override;
    void consumeResources() override;
    
    Organism* reproduce() override;

    bool canEat(const Organism* food) const;
    void eat(Organism* food);
    
    void move(Grid& grid);
    void hunt(Grid& grid, WorldManager& worldManager);
    void tryReproduce(Grid& grid, WorldManager& worldManager);

private:
    Position findBestMovePosition(Grid& grid);
    Organism* findNearestFood(Grid& grid);
    Organism* findAdjacentFood(Grid& grid);
};

#endif