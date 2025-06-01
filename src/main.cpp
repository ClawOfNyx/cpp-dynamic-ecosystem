#include <iostream>
#include <string>
#include <cstdlib> // For rand()
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
        
        cout << "Creating Plant object\n";
        Plant* plant = new Plant(5.0f, 100, 0.5f, 0.3f);
        cout << "Plant created with nutrients: " << plant->getNutrients() << "\n";
        
        world.addOrganism(plant, 5,5);
        cout << "Plant added to world" << endl;

        cout << "Creating Animal object\n";
        Animal* animal = new Animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 15.0f, 5);
        cout << "Animal created with nutrients: " << animal->getNutrients() << "\n";

        world.addOrganism(animal, 5,10);
        cout << "organism count: " << world.getOrganismCount() << endl;
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

        while (window.isOpen())
        {
                while (const std::optional event = window.pollEvent())
                {
                        
                if (event->is<sf::Event::Closed>())
                        window.close();
                }
        
                window.clear();

                world.update();
        
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
                
        }
        cout << "Press Enter to exit...";
        //cin.ignore();
        cin.get();
        
        return 0;

}