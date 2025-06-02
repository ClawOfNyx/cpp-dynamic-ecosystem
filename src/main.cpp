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

using namespace std;

int main() {
    int tileSize = 20;
    cout << "Starting debug program" << endl;
    
    cout << "Creating world (20x20)" << endl;
    WorldManager& world = WorldManager::getInstance(20, 20, 1.0f);
    cout << "World created successfully" << endl;
    
    cout << "Testing grid access" << endl;
    
    // Create multiple plants randomly distributed across the grid
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(0, 19);
    std::uniform_int_distribution<> yDist(0, 19);
    
    const int NUM_INITIAL_PLANTS = 15; // Spawn 15 plants initially
    
    cout << "Creating " << NUM_INITIAL_PLANTS << " Plant objects\n";
    for (int i = 0; i < NUM_INITIAL_PLANTS; ++i) {
        Plant* plant = new Plant(5.0f, 100, 0.5f, 0.3f);
        
        // Find an empty spot for the plant
        int attempts = 0;
        int x, y;
        do {
            x = xDist(gen);
            y = yDist(gen);
            attempts++;
        } while (!world.getGrid().getTile(x, y).isEmpty() && attempts < 50);
        
        if (attempts < 50) {
            world.addOrganism(plant, x, y);
            cout << "Plant " << i+1 << " added at (" << x << ", " << y << ")" << endl;
        } else {
            delete plant; // Clean up if we couldn't find a spot
            cout << "Could not find empty spot for plant " << i+1 << endl;
        }
    }

    cout << "Creating Animal object\n";
    Animal* animal = new Animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 15.0f, 5);
    cout << "Animal created with nutrients: " << animal->getNutrients() << "\n";

    // Find empty spot for animal
    int animalX, animalY;
    int attempts = 0;
    do {
        animalX = xDist(gen);
        animalY = yDist(gen);
        attempts++;
    } while (!world.getGrid().getTile(animalX, animalY).isEmpty() && attempts < 50);
    
    if (attempts < 50) {
        world.addOrganism(animal, animalX, animalY);
        cout << "Animal added at (" << animalX << ", " << animalY << ")" << endl;
    } else {
        cout << "Could not find empty spot for animal" << endl;
    }

    cout << "Total organism count: " << world.getOrganismCount() << endl;
    
    sf::RenderWindow window(
            sf::VideoMode(
                    {static_cast<unsigned int>(world.getGrid().getWidth() * tileSize),
                    static_cast<unsigned int>(world.getGrid().getHeight() * tileSize)}
            ),
            "SFML window"
    );
    
    const sf::Font font("fonts/arial.ttf");
    sf::Text text(font, "Hello SFML", 50);

    sf::Color plantColor = sf::Color::Green;
    sf::Color herbivoreColor = sf::Color::Blue;
    sf::Color carnivoreColor = sf::Color::Red;
    sf::Color emptyColor = sf::Color(200, 200, 200);

    // Add frame rate control variables
    sf::Clock clock;
    const float TARGET_FPS = 2.0f; // 2 updates per second for slow simulation
    const sf::Time frameTime = sf::seconds(1.0f / TARGET_FPS);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Only update simulation at target frame rate
        if (clock.getElapsedTime() >= frameTime) {
            world.update();
            clock.restart();
            
            // Optional: Print organism count every update
            cout << "Organisms alive: " << world.getOrganismCount() << endl;
        }

        window.clear();

        // Draw the grid
        for (int y = 0; y < world.getGrid().getHeight(); ++y) {
            for (int x = 0; x < world.getGrid().getWidth(); ++x) {
                const Tile& tile = world.getGrid().getTile(x, y);

                sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
                rect.setPosition(sf::Vector2f(static_cast<float>(x * tileSize), static_cast<float>(y * tileSize)));

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
                            rect.setFillColor(sf::Color::Magenta); // fallback
                            break;
                    }
                }

                window.draw(rect);
            }
        }
        
        window.display();
        
        // Small sleep to prevent excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    
    cout << "Press Enter to exit...";
    cin.get();
    
    return 0;
}