#include "catch2/catch_test_macros.hpp"
#include "Plant.h"
#include "Grid.h"
#include "WorldManager.h"
#include <cmath>

TEST_CASE("Plant construction and basic properties", "[Plant]") {
    SECTION("Plant construction with valid parameters") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        
        REQUIRE(plant.getType() == OrganismType::PLANT);
        REQUIRE(plant.getNutrients() == 10.0f);
        REQUIRE(plant.getGrowthRate() == 0.5f);
        REQUIRE(plant.getNutrientAbsorptionRate() == 0.3f);
        REQUIRE(plant.getAge() == 0);
        REQUIRE_FALSE(plant.isDead());
    }
    
    SECTION("Plant property setters") {
        Plant plant(5.0f, 50, 0.2f, 0.1f);
        
        plant.setGrowthRate(0.8f);
        plant.setNutrientAbsorptionRate(0.6f);
        
        REQUIRE(plant.getGrowthRate() == 0.8f);
        REQUIRE(plant.getNutrientAbsorptionRate() == 0.6f);
    }
}

TEST_CASE("Plant reproduction readiness", "[Plant]") {
    SECTION("Plant not ready to reproduce with low nutrients") {
        Plant plant(5.0f, 100, 0.5f, 0.3f);
        REQUIRE_FALSE(plant.isReadyToReproduce());
    }
    
    SECTION("Plant ready to reproduce with sufficient nutrients") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        REQUIRE(plant.isReadyToReproduce());
    }
    
    SECTION("Plant at exact threshold") {
        Plant plant(8.0f, 100, 0.5f, 0.3f);
        REQUIRE_FALSE(plant.isReadyToReproduce()); 
    }
    
    SECTION("Plant just above threshold") {
        Plant plant(8.1f, 100, 0.5f, 0.3f);
        REQUIRE(plant.isReadyToReproduce());
    }
}

TEST_CASE("Plant nutrient absorption", "[Plant]") {
    SECTION("Nutrient absorption calculation") {
        Plant plant(5.0f, 100, 0.5f, 0.3f);
        float initialNutrients = plant.getNutrients();
        
        plant.absorbNutrients();
        
        float expectedAbsorption = 0.3f * 0.5f * 2.0f; 
        float expectedTotal = initialNutrients + expectedAbsorption;
        
        REQUIRE(std::abs(plant.getNutrients() - expectedTotal) < 0.001f);
    }
    
    SECTION("Multiple absorption cycles") {
        Plant plant(2.0f, 100, 1.0f, 0.4f);
        
        plant.absorbNutrients();
        plant.absorbNutrients();
        
        REQUIRE(std::abs(plant.getNutrients() - 3.6f) < 0.001f);
    }
}

TEST_CASE("Plant reproduction", "[Plant]") {
    SECTION("Successful reproduction creates offspring") {
        Plant parent(12.0f, 100, 0.6f, 0.4f);
        
        Organism* offspring = parent.reproduce();
        
        REQUIRE(offspring != nullptr);
        REQUIRE(offspring->getType() == OrganismType::PLANT);
        
        Plant* plantOffspring = dynamic_cast<Plant*>(offspring);
        REQUIRE(plantOffspring != nullptr);
        
        // Check offspring has reasonable stats (with variation)
        REQUIRE(plantOffspring->getNutrients() > 0);
        REQUIRE(plantOffspring->getGrowthRate() > 0);
        REQUIRE(plantOffspring->getNutrientAbsorptionRate() > 0);
        
        // Parent should have consumed nutrients for reproduction
        REQUIRE(parent.getNutrients() < 12.0f);
        
        delete offspring;
    }
    
    SECTION("Reproduction fails when not ready") {
        Plant plant(5.0f, 100, 0.5f, 0.3f); // Below reproduction threshold
        
        Organism* offspring = plant.reproduce();
        
        REQUIRE(offspring == nullptr);
    }
    
    SECTION("Reproduction consumes parent nutrients") {
        Plant parent(10.0f, 100, 0.5f, 0.3f);
        float initialNutrients = parent.getNutrients();
        
        Organism* offspring = parent.reproduce();
        
        // Should consume spreadingThreshold / 2 = 8.0f / 2 = 4.0f
        REQUIRE(parent.getNutrients() == initialNutrients - 4.0f);
        
        delete offspring;
    }
}

TEST_CASE("Plant resource consumption", "[Plant]") {
    SECTION("Plants don't consume resources like animals") {
        Plant plant(10.0f, 100, 0.5f, 0.3f);
        float initialNutrients = plant.getNutrients();
        
        plant.consumeResources();
        
        // Should not change nutrients
        REQUIRE(plant.getNutrients() == initialNutrients);
    }
}
