#include "catch2/catch_test_macros.hpp"
#include "Organism.h"
#include "Plant.h"
#include "Animal.h"

TEST_CASE("Organism basic functionality", "[Organism]") {
    SECTION("Organism construction") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        
        REQUIRE(plant.getType() == OrganismType::PLANT);
        REQUIRE(plant.getNutrients() == 10.0f);
        REQUIRE(plant.getAge() == 0);
        REQUIRE_FALSE(plant.isDead());
    }
    
    SECTION("Nutrient management") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        
        // Add nutrients
        plant.addNutrients(5.0f);
        REQUIRE(plant.getNutrients() == 15.0f);
        
        // Consume nutrients
        plant.consumeNutrients(3.0f);
        REQUIRE(plant.getNutrients() == 12.0f);
        
        plant.consumeNutrients(20.0f);
        REQUIRE(plant.getNutrients() == 0.0f);
    }
    
    SECTION("Age management") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        
        REQUIRE(plant.getAge() == 0);
        
        plant.incrementAge();
        REQUIRE(plant.getAge() == 1);
        
        for (int i = 0; i < 10; ++i) {
            plant.incrementAge();
        }
        REQUIRE(plant.getAge() == 11);
    }
    
    SECTION("Death conditions") {
        SECTION("Death by age") {
            Plant plant(10.0f, 5, 0.5f, 0.3f);
            
            REQUIRE_FALSE(plant.isDead());
            
            // Age to maximum
            for (int i = 0; i < 5; ++i) {
                plant.incrementAge();
            }
            
            REQUIRE(plant.isDead());
        }
        
        SECTION("Death by starvation") {
            Plant plant(5.0f, 100, 0.5f, 0.3f);
            
            REQUIRE_FALSE(plant.isDead());
            
            plant.consumeNutrients(5.0f);
            
            REQUIRE(plant.isDead());
        }
        
        SECTION("Alive with nutrients and age") {
            Plant plant(10.0f, 100, 0.5f, 0.3f);
            
            plant.incrementAge();
            REQUIRE_FALSE(plant.isDead());
            
            plant.consumeNutrients(5.0f);
            REQUIRE_FALSE(plant.isDead());
        }
    }
}

TEST_CASE("Organism position management", "[Organism]") {
    Plant plant(10.0f, 100, 0.5f, 0.3f);
    
    SECTION("Initial position is null") {
        // Note: This test assumes position starts as null
    }
    
    SECTION("Setting and getting position") {
        Position pos(5, 7);
        plant.setPosition(pos);
        
        const Position& retrievedPos = plant.getPosition();
        REQUIRE(retrievedPos.getX() == 5);
        REQUIRE(retrievedPos.getY() == 7);
    }
    
    SECTION("Updating position") {
        Position pos1(3, 4);
        Position pos2(8, 9);
        
        plant.setPosition(pos1);
        REQUIRE(plant.getPosition().getX() == 3);
        REQUIRE(plant.getPosition().getY() == 4);
        
        plant.setPosition(pos2);
        REQUIRE(plant.getPosition().getX() == 8);
        REQUIRE(plant.getPosition().getY() == 9);
    }
}

TEST_CASE("Organism type identification", "[Organism]") {
    SECTION("Plant type identification") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        REQUIRE(plant.getType() == OrganismType::PLANT);
    }
    
    SECTION("Animal type identification") {
        Animal animal(15.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 20.0f, 5);
        REQUIRE(animal.getType() == OrganismType::ANIMAL);
    }
}