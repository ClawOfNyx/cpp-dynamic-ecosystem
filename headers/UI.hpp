#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class UI {
public:
    struct SimulationSettings {
        int herbivoreCount = 10;
        int carnivoreCount = 5;
        int plantCount = 50;
        float simulationSpeed = 1.0f;
        float plantGrowthSpeed = 1.0f;
        int gridSize = 20;
        int tileSize = 20;
    };

    UI();
    bool run();
    SimulationSettings getSettings() const;

private:
    sf::RenderWindow window;
    sf::Font font;
    bool isStarted;
    
    // Input handling
    std::string activeInputField;
    
    SimulationSettings settings;
    
    // UI Elements
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    
    // Input fields and their backgrounds
    sf::RectangleShape herbivoreInput;
    sf::RectangleShape carnivoreInput;
    sf::RectangleShape plantInput;
    sf::RectangleShape speedInput;
    sf::RectangleShape growthInput;
    sf::RectangleShape gridSizeInput;
    sf::RectangleShape tileSizeInput;
    
    // Text objects - no longer default constructible in SFML 3.x
    sf::Text herbivoreText;
    sf::Text carnivoreText;
    sf::Text plantText;
    sf::Text speedText;
    sf::Text growthText;
    sf::Text gridSizeText;
    sf::Text tileSizeText;
    
    void initializeUI();
    void createInputField(sf::RectangleShape& inputField, sf::Text& text, const std::string& label, 
                     float yPos, float value);
    void updateInputField(sf::RectangleShape& inputField, sf::Text& text, const std::string& label, 
                     const std::string& input, float& value);
    void handleEvents();
    void draw();
};

#endif // UI_HPP