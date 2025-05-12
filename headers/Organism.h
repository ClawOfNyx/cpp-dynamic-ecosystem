#ifndef ORGANISM_H
#define ORGANISM_H

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

public:
    Organism(OrganismType type, float nutrients, int maxLifespan);
    virtual ~Organism() = default;

    // Non-virtual methods 
    OrganismType getType() const;
    float getNutrients() const;
    void addNutrients(float amount);
    void consumeNutrients(float amount);
    int getAge() const;
    void incrementAge();
    bool isDead() const;
    
    // Pure virtual methods
    virtual void update() = 0;
    virtual bool isReadyToReproduce() const = 0;
    virtual void consumeResources() = 0;
    
    virtual Organism* reproduce();
};

#endif