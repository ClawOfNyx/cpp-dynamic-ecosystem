#include <SFML/Graphics.hpp>
#include "Plant.h"

int main() {
    auto window = sf::RenderWindow(sf::VideoMode({1080u, 720u}), "Simulation");

    sf::Font font;
    if (!font.openFromFile("fonts\\Arial.ttf")) {
        return -1;
    }

    Plant plant(5.0f, 10, 0.5f, 1.0f);

    sf::Text text(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);
    text.setPosition({10.f, 10.f});

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Build the text to display
        std::ostringstream stateStream;
        stateStream << "Initial Plant State:\n"
                    << "Type: " << (plant.getType() == OrganismType::PLANT ? "PLANT" : "ANIMAL") << "\n"
                    << "Nutrients: " << plant.getNutrients() << "\n"
                    << "Age: " << plant.getAge() << "\n"
                    << "Ready to reproduce? " << (plant.isReadyToReproduce() ? "Yes" : "No") << "\n"
                    << "Is dead? " << (plant.isDead() ? "Yes" : "No") << "\n";


        text.setString(stateStream.str());

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}