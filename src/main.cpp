#include <iostream>
#include <string>
#include <cstdlib> // For rand()
#include <thread>  // For sleep
#include <chrono>  // For chrono literals
#include <random>  // For better random number generation
#include "WorldManager.h"
#include "Animal.h"
#include "Plant.h"
#include "Position.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../headers/UI.hpp"

using namespace std;
    
const int NUM_INITIAL_PLANTS = 1000; // Spawn 15 plants initially
const int TILE_SIZE = 20;
const int NUM_INITIAL_CARNIVORES = 1;
const int NUM_INITIAL_HERBIVORES = 1; // Start with 3 herbivores
const int WINDOW_DIMENSION = 100;

// Global spreading threshold - controls how fast plants spread
// Lower values = faster spreading, Higher values = slower spreading
const float GLOBAL_SPREADING_THRESHOLD = 12.0f;

int main() {
    // Create and run the UI to get simulation settings
    UI ui;
    if (!ui.run()) {
        std::cout << "Simulation cancelled" << std::endl;
        return 0;
    }
    
    UI::SimulationSettings settings = ui.getSettings();
    
    cout << "Starting simulation with settings:" << endl;
    cout << "Herbivores: " << settings.herbivoreCount << endl;
    cout << "Carnivores: " << settings.carnivoreCount << endl;
    cout << "Plants: " << settings.plantCount << endl;
    cout << "Simulation Speed: " << settings.simulationSpeed << endl;
    cout << "Plant Growth Speed: " << settings.plantGrowthSpeed << endl;
    cout << "Grid Size: " << settings.gridSize << endl;
    cout << "Tile Size: " << settings.tileSize << endl;
    
    // Create the world manager with grid dimensions and base nutrients
    WorldManager& world = WorldManager::getInstance(settings.gridSize, settings.gridSize, 1.0f);
    
    // Set the global spreading threshold based on plant growth speed
    // Higher growth speed = lower threshold (faster spreading)
    // Use an inverse relationship: threshold = baseThreshold / growthSpeed;
    float spreadingThreshold = settings.plantGrowthSpeed > 0 ? GLOBAL_SPREADING_THRESHOLD / settings.plantGrowthSpeed : GLOBAL_SPREADING_THRESHOLD;
    world.setGlobalSpreadingThreshold(spreadingThreshold);
    
    // Create random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, settings.gridSize - 1);
    std::uniform_int_distribution<> yDist(0, settings.gridSize - 1);
    
    // Create distributions for varying plant properties
    std::uniform_real_distribution<float> growthRateDist(0.3f, 0.8f);
    std::uniform_real_distribution<float> absorptionRateDist(0.2f, 0.5f);
    std::uniform_real_distribution<float> initialNutrientsDist(3.0f, 8.0f);
    
    // Create plants
    for (int i = 0; i < settings.plantCount; ++i) {
        float growthRate = growthRateDist(gen);
        float absorptionRate = absorptionRateDist(gen);
        float initialNutrients = initialNutrientsDist(gen);
        float plantSpreadingThreshold = 12.0f * (1.0f + growthRate);
        
        Plant* plant = new Plant(initialNutrients, 100, growthRate, absorptionRate, plantSpreadingThreshold);
        
        int attempts = 0;
        int x, y;
        do {
            x = xDist(gen);
            y = yDist(gen);
            attempts++;
        } while (!world.getGrid().getTile(x, y).isEmpty() && attempts < 50);
        
        if (attempts < 50) {
            world.addOrganism(plant, x, y);
        } else {
            delete plant;
        }
    }

    // Create herbivores
    for (int i = 0; i < settings.herbivoreCount; ++i) {
        Animal* herbivore = new Animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 15.0f, 5);
        
        int herbivoreX, herbivoreY;
        int herbAttempts = 0;
        do {
            herbivoreX = xDist(gen);
            herbivoreY = yDist(gen);
            herbAttempts++;
        } while (!world.getGrid().getTile(herbivoreX, herbivoreY).isEmpty() && herbAttempts < 50);
        
        if (herbAttempts < 50) {
            world.addOrganism(herbivore, herbivoreX, herbivoreY);
        } else {
            delete herbivore;
        }
    }

    // Create carnivores
    for (int i = 0; i < settings.carnivoreCount; ++i) {
        Animal* carnivore = new Animal(18.0f, 70, 3, 7, AnimalType::CARNIVORE, 1.5f, 24.0f, 8);
        
        int carnivoreX, carnivoreY;
        int carnAttempts = 0;
        do {
            carnivoreX = xDist(gen);
            carnivoreY = yDist(gen);
            carnAttempts++;
        } while (!world.getGrid().getTile(carnivoreX, carnivoreY).isEmpty() && carnAttempts < 50);
        
        if (carnAttempts < 50) {
            world.addOrganism(carnivore, carnivoreX, carnivoreY);
        } else {
            delete carnivore;
        }
    }

    cout << "Total organism count: " << world.getOrganismCount() << endl;
    
    sf::RenderWindow window(
            sf::VideoMode(
                    {static_cast<unsigned int>(world.getGrid().getWidth() * settings.tileSize),
                     static_cast<unsigned int>(world.getGrid().getHeight() * settings.tileSize)}
            ),
            "Ecosystem Simulation"
    );

    sf::Color plantColor = sf::Color::Green;
    sf::Color herbivoreColor = sf::Color::Blue;
    sf::Color carnivoreColor = sf::Color::Red;
    sf::Color emptyColor = sf::Color(200, 200, 200);

    sf::Clock clock;
    const float TARGET_FPS = 2.0f * settings.simulationSpeed;
    const sf::Time frameTime = sf::seconds(1.0f / TARGET_FPS);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (clock.getElapsedTime() >= frameTime) {
            world.update();
            clock.restart();
            cout << "Organisms alive: " << world.getOrganismCount() << endl;
        }

        window.clear();

        for (int y = 0; y < world.getGrid().getHeight(); ++y) {
            for (int x = 0; x < world.getGrid().getWidth(); ++x) {
                const Tile& tile = world.getGrid().getTile(x, y);

                sf::RectangleShape rect(sf::Vector2f(settings.tileSize, settings.tileSize));
                rect.setPosition(sf::Vector2f(static_cast<float>(x * settings.tileSize), 
                                            static_cast<float>(y * settings.tileSize)));

                if (tile.isEmpty()) {
                    rect.setFillColor(emptyColor);
                } else {
                    Organism* occupant = tile.getOccupant();
                    switch (occupant->getType()) {
                        case OrganismType::PLANT:
                            rect.setFillColor(plantColor);
                            break;
                        case OrganismType::ANIMAL: {
                            Animal* animal = dynamic_cast<Animal*>(occupant);
                            if (animal != nullptr) {
                                if (animal->getAnimalType() == AnimalType::HERBIVORE) {
                                    rect.setFillColor(herbivoreColor);
                                } else if (animal->getAnimalType() == AnimalType::CARNIVORE) {
                                    rect.setFillColor(carnivoreColor);
                                }
                            }
                            break;
                        }
                        default:
                            rect.setFillColor(sf::Color::Magenta);
                            break;
                    }
                }

                window.draw(rect);
            }
        }
        
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    return 0;
}