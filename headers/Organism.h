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

    OrganismType getType() const;
    float getNutrients() const;
    void addNutrients(float amount);
    void consumeNutrients(float amount);
    int getAge() const;
    void incrementAge();
    virtual bool isReadyToReproduce() const;
    bool isDead() const;
};

#endif
