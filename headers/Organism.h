#ifndef ORGANISM_H
#define ORGANISM_H

#include "Position.h"

class Grid;

enum class OrganismType {
    PLANT,
    ANIMAL
};

class Organism {
protected:
    OrganismType type;
    float nutrients;
    int age;
    int maxLifespan;
    Position* position;

public:
    Organism(OrganismType type, float nutrients, int maxLifespan);
    virtual ~Organism();

    // Non-virtual methods 
    OrganismType getType() const;
    float getNutrients() const;
    void addNutrients(float amount);
    void consumeNutrients(float amount);
    int getAge() const;
    void incrementAge();
    bool isDead() const;
    
    const Position& getPosition() const;
    void setPosition(const Position& newPos);
    
    virtual void update(Grid& grid) = 0;
    virtual bool isReadyToReproduce() const = 0;
    virtual void consumeResources() = 0;
    
    virtual Organism* reproduce();
};

#endif