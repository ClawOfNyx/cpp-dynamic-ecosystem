#include "catch2/catch_test_macros.hpp"
#include "Animal.h"
#include "Plant.h"
#include "Grid.h"
#include "WorldManager.h"
#include <cmath>
#include <memory>

TEST_CASE("Animal functionality", "[Animal]") {
    SECTION("Animal creation") {
        Animal herbivore(20.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.5f, 25.0f, 10);
        
        REQUIRE(herbivore.getType() == OrganismType::ANIMAL);
        REQUIRE(herbivore.getNutrients() == 20.0f);
        REQUIRE(herbivore.getAnimalType() == AnimalType::HERBIVORE);
        REQUIRE(herbivore.getMovementSpeed() == 2);
        REQUIRE(herbivore.getVisionDistance() == 5);
        REQUIRE(herbivore.getNutrientRequirement() == 1.5f);
        REQUIRE(herbivore.getReproductionNutrientThreshold() == 25.0f);
        REQUIRE(herbivore.getMass() == 10);
    }
    
    SECTION("Animal reproduction readiness") {
        Animal animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.5f, 15.0f, 10);
        REQUIRE(animal.isReadyToReproduce() == false);
        
        animal.addNutrients(10.0f); // Total 20.0f, above threshold of 15.0f
        REQUIRE(animal.isReadyToReproduce() == true);
    }
    
    SECTION("Animal reproduction") {
        Animal animal(30.0f, 80, 2, 5, AnimalType::CARNIVORE, 1.5f, 20.0f, 8);
        
        Organism* offspring = animal.reproduce();
        REQUIRE(offspring != nullptr);
        REQUIRE(offspring->getType() == OrganismType::ANIMAL);
        
        Animal* babyAnimal = dynamic_cast<Animal*>(offspring);
        REQUIRE(babyAnimal != nullptr);
        REQUIRE(babyAnimal->getAnimalType() == AnimalType::CARNIVORE);
        
        delete offspring;
    }
    
    SECTION("Animal resource consumption") {
        Animal animal(10.0f, 80, 2, 5, AnimalType::OMNIVORE, 2.0f, 15.0f, 12);
        float initialNutrients = animal.getNutrients();
        
        animal.consumeResources();
        REQUIRE(animal.getNutrients() < initialNutrients);
        REQUIRE(animal.getNutrients() == (initialNutrients - 2.0f));
    }
    
    SECTION("Animal diet restrictions") {
        Animal herbivore(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 8);
        Animal carnivore(15.0f, 80, 2, 5, AnimalType::CARNIVORE, 1.5f, 20.0f, 12);
        Animal omnivore(15.0f, 80, 2, 5, AnimalType::OMNIVORE, 1.2f, 20.0f, 10);
        
        Plant plant(10.0f, 50, 0.5f, 0.3f);
        Animal prey(8.0f, 60, 1, 3, AnimalType::HERBIVORE, 0.8f, 12.0f, 5);
        
        // Herbivore can eat plants but not animals
        REQUIRE(herbivore.canEat(&plant) == true);
        REQUIRE(herbivore.canEat(&prey) == false);
        
        // Carnivore can eat animals but not plants
        REQUIRE(carnivore.canEat(&plant) == false);
        REQUIRE(carnivore.canEat(&prey) == true);
        
        // Omnivore can eat both
        REQUIRE(omnivore.canEat(&plant) == true);
        REQUIRE(omnivore.canEat(&prey) == true);
    }
    
    SECTION("Animal eating") {
        Animal herbivore(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 8);
        Plant plant(5.0f, 50, 0.5f, 0.3f);
        
        float initialNutrients = herbivore.getNutrients();
        herbivore.eat(&plant);
        
        REQUIRE(herbivore.getNutrients() > initialNutrients);
        REQUIRE(herbivore.getNutrients() == (initialNutrients + 5.0f));
    }
}

TEST_CASE("Animal property setters", "[Animal]") {
    Animal animal(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 8);
    
    SECTION("Movement speed setter") {
        animal.setMovementSpeed(5);
        REQUIRE(animal.getMovementSpeed() == 5);
    }
    
    SECTION("Vision distance setter") {
        animal.setVisionDistance(10);
        REQUIRE(animal.getVisionDistance() == 10);
    }
    
    SECTION("Animal type setter") {
        animal.setAnimalType(AnimalType::CARNIVORE);
        REQUIRE(animal.getAnimalType() == AnimalType::CARNIVORE);
    }
    
    SECTION("Nutrient requirement setter") {
        animal.setNutrientRequirement(2.5f);
        REQUIRE(animal.getNutrientRequirement() == 2.5f);
    }
    
    SECTION("Reproduction threshold setter") {
        animal.setReproductionNutrientThreshold(30.0f);
        REQUIRE(animal.getReproductionNutrientThreshold() == 30.0f);
    }
    
    SECTION("Mass setter") {
        animal.setMass(15);
        REQUIRE(animal.getMass() == 15);
    }
}

TEST_CASE("Animal reproduction with nutrient consumption", "[Animal]") {
    SECTION("Reproduction consumes correct nutrients") {
        Animal parent(50.0f, 100, 2, 5, AnimalType::HERBIVORE, 1.0f, 30.0f, 10);
        float initialNutrients = parent.getNutrients();
        
        Organism* offspring = parent.reproduce();
        
        REQUIRE(offspring != nullptr);
        REQUIRE(parent.getNutrients() == (initialNutrients - 15.0f));
        
        delete offspring;
    }
    
    SECTION("Reproduction fails when nutrients insufficient") {
        Animal parent(20.0f, 100, 2, 5, AnimalType::HERBIVORE, 1.0f, 30.0f, 10);
        
        Organism* offspring = parent.reproduce();
        REQUIRE(offspring == nullptr);
    }
    
    SECTION("Offspring has correct starting nutrients") {
        Animal parent(50.0f, 100, 2, 5, AnimalType::CARNIVORE, 1.5f, 30.0f, 12);
        
        Organism* offspring = parent.reproduce();
        REQUIRE(offspring != nullptr);
        
        Animal* baby = dynamic_cast<Animal*>(offspring);
        REQUIRE(baby != nullptr);
        // Offspring starts with reproductionNutrientThreshold / 2 = 15.0f
        REQUIRE(baby->getNutrients() == 15.0f);
        
        delete offspring;
    }
    
    SECTION("Offspring inherits parent properties with variation") {
        Animal parent(50.0f, 100, 4, 6, AnimalType::OMNIVORE, 2.0f, 30.0f, 15);
        
        Organism* offspring = parent.reproduce();
        REQUIRE(offspring != nullptr);
        
        Animal* baby = dynamic_cast<Animal*>(offspring);
        REQUIRE(baby != nullptr);
        
        REQUIRE(baby->getAnimalType() == AnimalType::OMNIVORE);
        REQUIRE(baby->getVisionDistance() == 6);
        REQUIRE(baby->getMass() == 15);
        
        REQUIRE(baby->getMovementSpeed() >= 3); 
        REQUIRE(baby->getMovementSpeed() <= 5);
        REQUIRE(baby->getNutrientRequirement() >= 1.8f);
        REQUIRE(baby->getNutrientRequirement() <= 2.4f);
        
        delete offspring;
    }
}

TEST_CASE("Animal eating behavior edge cases", "[Animal]") {
    SECTION("Cannot eat null organism") {
        Animal herbivore(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 8);
        
        REQUIRE(herbivore.canEat(nullptr) == false);
        
        float initialNutrients = herbivore.getNutrients();
        herbivore.eat(nullptr);
        REQUIRE(herbivore.getNutrients() == initialNutrients);
    }
    
    SECTION("Cannot eat incompatible food types") {
        Animal herbivore(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 8);
        Animal prey(10.0f, 60, 1, 3, AnimalType::HERBIVORE, 0.8f, 12.0f, 5);
        
        float initialNutrients = herbivore.getNutrients();
        herbivore.eat(&prey);
        REQUIRE(herbivore.getNutrients() == initialNutrients);
    }
    
    SECTION("Eating adds correct nutrients") {
        Animal omnivore(20.0f, 80, 2, 5, AnimalType::OMNIVORE, 1.2f, 25.0f, 10);
        Plant plant(7.5f, 50, 0.5f, 0.3f);
        
        float initialNutrients = omnivore.getNutrients();
        omnivore.eat(&plant);
        
        REQUIRE(omnivore.getNutrients() == (initialNutrients + 7.5f));
    }
}

TEST_CASE("Animal aging and death", "[Animal]") {
    SECTION("Animal dies from old age") {
        Animal animal(50.0f, 5, 2, 5, AnimalType::HERBIVORE, 1.0f, 25.0f, 8);
        
        REQUIRE_FALSE(animal.isDead());
        
        // Age to maximum
        for (int i = 0; i < 5; ++i) {
            animal.incrementAge();
        }
        
        REQUIRE(animal.isDead());
    }
    
    SECTION("Animal dies from starvation") {
        Animal animal(3.0f, 100, 2, 5, AnimalType::HERBIVORE, 1.0f, 25.0f, 8);
        
        REQUIRE_FALSE(animal.isDead());
        
        // Consume all nutrients
        animal.consumeNutrients(3.0f);
        
        REQUIRE(animal.isDead());
    }
    
    SECTION("Animal survives with sufficient nutrients and age") {
        Animal animal(20.0f, 100, 2, 5, AnimalType::HERBIVORE, 1.0f, 25.0f, 8);
        
        animal.incrementAge();
        animal.consumeNutrients(5.0f);
        
        REQUIRE_FALSE(animal.isDead());
    }
}