#include "../headers/UI.hpp"
#include <iostream>
#include <sstream>
#include <cstdint>  // Add this include for std::uint32_t

UI::UI() : window(sf::VideoMode(sf::Vector2u(800, 600)), "Ecosystem Simulation Setup"), 
           font(),
           isStarted(false),
           activeInputField(""),
           settings{400, 300, 2000, 1.0f, 12.0f, 100, 20},
           startButtonText(font),
           herbivoreText(font),
           carnivoreText(font),
           plantText(font),
           speedText(font),
           growthText(font),
           gridSizeText(font),
           tileSizeText(font) {
    initializeUI();
}

void UI::initializeUI() {
    // Load font
    if (!font.openFromFile("../fonts/arial.ttf")) {
        std::cout << "Failed to load font" << std::endl;
    }
    
    // Initialize active input field
    activeInputField = "";
    
    // Create input fields and text
    createInputField(herbivoreInput, herbivoreText, "Herbivores: ", 100.0f, settings.herbivoreCount);
    createInputField(carnivoreInput, carnivoreText, "Carnivores: ", 150.0f, settings.carnivoreCount);
    createInputField(plantInput, plantText, "Plants: ", 200.0f, settings.plantCount);
    createInputField(speedInput, speedText, "Simulation Speed: ", 250.0f, settings.simulationSpeed);
    createInputField(growthInput, growthText, "Plant Growth: ", 300.0f, settings.plantGrowthSpeed);
    createInputField(gridSizeInput, gridSizeText, "Grid Size: ", 350.0f, settings.gridSize);
    createInputField(tileSizeInput, tileSizeText, "Tile Size: ", 400.0f, settings.tileSize);
    
    // Create start button
    startButton.setSize(sf::Vector2f(200.0f, 50.0f));
    startButton.setPosition(sf::Vector2f(300.0f, 500.0f));
    startButton.setFillColor(sf::Color::Green);
    
    startButtonText.setFont(font);
    startButtonText.setString("Start Simulation");
    startButtonText.setCharacterSize(20);
    startButtonText.setFillColor(sf::Color::Black);
    startButtonText.setPosition(sf::Vector2f(320.0f, 510.0f));
}

void UI::createInputField(sf::RectangleShape& inputField, sf::Text& text, const std::string& label, 
                     float yPos, float value) {
    // Create input field background
    inputField.setSize(sf::Vector2f(100.0f, 30.0f));
    inputField.setPosition(sf::Vector2f(250.0f, yPos));
    inputField.setFillColor(sf::Color::White);
    inputField.setOutlineColor(sf::Color::Black);
    inputField.setOutlineThickness(2.0f);
    
    // Format value string based on whether it's an integer or float
    std::string valueStr;
    if (value == static_cast<int>(value)) {
        valueStr = std::to_string(static_cast<int>(value));
    } else {
        std::ostringstream ss;
        ss.precision(2);
        ss << std::fixed << value;
        valueStr = ss.str();
    }
    
    // Set up the label text
    text.setString(label + valueStr);
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(50.0f, yPos));
}

void UI::updateInputField(sf::RectangleShape& inputField, sf::Text& text, const std::string& label, 
                     const std::string& input, float& value) {
    try {
        // Handle empty input case
        if (input.empty()) {
            value = 0.0f;
            text.setString(label + "0");
            return;
        }
        
        // Convert input string to appropriate value type
        if (input.find(".") != std::string::npos) {
            // Input contains decimal point - parse as float
            value = std::stof(input);
        } else {
            // Parse as integer and convert to float
            value = static_cast<float>(std::stoi(input));
        }
        
        // Format display string
        std::string valueStr;
        if (value == static_cast<int>(value)) {
            valueStr = std::to_string(static_cast<int>(value));
        } else {
            std::ostringstream ss;
            ss.precision(2);
            ss << std::fixed << value;
            valueStr = ss.str();
        }
        
        // Update text display
        text.setString(label + valueStr);
    } catch (const std::exception& e) {
        // If conversion fails, keep the old value but don't show error for empty strings
        if (!input.empty()) {
            std::cout << "Invalid input: " << e.what() << std::endl;
        }
    }
}

void UI::handleEvents() {
    // SFML 3.x Event handling - pollEvent now returns optional
    if (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            
            // Check if start button is clicked
            if (mousePos.x >= startButton.getPosition().x && 
                mousePos.x <= startButton.getPosition().x + startButton.getSize().x &&
                mousePos.y >= startButton.getPosition().y && 
                mousePos.y <= startButton.getPosition().y + startButton.getSize().y) {
                isStarted = true;
                return;
            }
            
            // Check if any input field is clicked
            if (mousePos.x >= herbivoreInput.getPosition().x && 
                mousePos.x <= herbivoreInput.getPosition().x + herbivoreInput.getSize().x &&
                mousePos.y >= herbivoreInput.getPosition().y && 
                mousePos.y <= herbivoreInput.getPosition().y + herbivoreInput.getSize().y) {
                activeInputField = "herbivore";
            }
            else if (mousePos.x >= carnivoreInput.getPosition().x && 
                     mousePos.x <= carnivoreInput.getPosition().x + carnivoreInput.getSize().x &&
                     mousePos.y >= carnivoreInput.getPosition().y && 
                     mousePos.y <= carnivoreInput.getPosition().y + carnivoreInput.getSize().y) {
                activeInputField = "carnivore";
            }
            else if (mousePos.x >= plantInput.getPosition().x && 
                     mousePos.x <= plantInput.getPosition().x + plantInput.getSize().x &&
                     mousePos.y >= plantInput.getPosition().y && 
                     mousePos.y <= plantInput.getPosition().y + plantInput.getSize().y) {
                activeInputField = "plant";
            }
            else if (mousePos.x >= speedInput.getPosition().x && 
                     mousePos.x <= speedInput.getPosition().x + speedInput.getSize().x &&
                     mousePos.y >= speedInput.getPosition().y && 
                     mousePos.y <= speedInput.getPosition().y + speedInput.getSize().y) {
                activeInputField = "speed";
            }
            else if (mousePos.x >= growthInput.getPosition().x && 
                     mousePos.x <= growthInput.getPosition().x + growthInput.getSize().x &&
                     mousePos.y >= growthInput.getPosition().y && 
                     mousePos.y <= growthInput.getPosition().y + growthInput.getSize().y) {
                activeInputField = "growth";
            }
            else if (mousePos.x >= gridSizeInput.getPosition().x && 
                     mousePos.x <= gridSizeInput.getPosition().x + gridSizeInput.getSize().x &&
                     mousePos.y >= gridSizeInput.getPosition().y && 
                     mousePos.y <= gridSizeInput.getPosition().y + gridSizeInput.getSize().y) {
                activeInputField = "grid";
            }
            else if (mousePos.x >= tileSizeInput.getPosition().x && 
                     mousePos.x <= tileSizeInput.getPosition().x + tileSizeInput.getSize().x &&
                     mousePos.y >= tileSizeInput.getPosition().y && 
                     mousePos.y <= tileSizeInput.getPosition().y + tileSizeInput.getSize().y) {
                activeInputField = "tile";
            }
            else {
                // Clicked outside any input field
                activeInputField = "";
            }
        }
        else if (auto textEvent = event->getIf<sf::Event::TextEntered>()) {
            if (!activeInputField.empty()) {
                std::uint32_t unicode = textEvent->unicode;

                
                // Handle backspace (unicode 8)
                if (unicode == 8) {
                    // Get current text and remove last character if not empty
                    std::string currentText;
                    if (activeInputField == "herbivore") {
                        std::string label = "Herbivores: ";
                        currentText = herbivoreText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        float value = 0.0f;
                        updateInputField(herbivoreInput, herbivoreText, label, currentText, value);
                        settings.herbivoreCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "carnivore") {
                        std::string label = "Carnivores: ";
                        currentText = carnivoreText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        float value = 0.0f;
                        updateInputField(carnivoreInput, carnivoreText, label, currentText, value);
                        settings.carnivoreCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "plant") {
                        std::string label = "Plants: ";
                        currentText = plantText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        float value = 0.0f;
                        updateInputField(plantInput, plantText, label, currentText, value);
                        settings.plantCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "speed") {
                        std::string label = "Simulation Speed: ";
                        currentText = speedText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        updateInputField(speedInput, speedText, label, currentText, settings.simulationSpeed);
                    }
                    else if (activeInputField == "growth") {
                        std::string label = "Plant Growth: ";
                        currentText = growthText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        updateInputField(growthInput, growthText, label, currentText, settings.plantGrowthSpeed);
                    }
                    else if (activeInputField == "grid") {
                        std::string label = "Grid Size: ";
                        currentText = gridSizeText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        float value = 0.0f;
                        updateInputField(gridSizeInput, gridSizeText, label, currentText, value);
                        settings.gridSize = static_cast<int>(value);
                    }
                    else if (activeInputField == "tile") {
                        std::string label = "Tile Size: ";
                        currentText = tileSizeText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        // Allow removing all characters
                        if (!currentText.empty()) {
                            currentText.pop_back();
                        }
                        float value = 0.0f;
                        updateInputField(tileSizeInput, tileSizeText, label, currentText, value);
                        settings.tileSize = static_cast<int>(value);
                    }
                }
                // Handle numeric input (0-9) and decimal point
                else if ((unicode >= '0' && unicode <= '9') || unicode == '.') {
                    char inputChar = static_cast<char>(unicode);
                    std::string input(1, inputChar);
                    
                    if (activeInputField == "herbivore") {
                        std::string label = "Herbivores: ";
                        std::string currentText = herbivoreText.getString().toAnsiString();
                        // Make sure we're extracting the correct part by checking the length
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        float value = 0.0f;
                        updateInputField(herbivoreInput, herbivoreText, label, currentText, value);
                        settings.herbivoreCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "carnivore") {
                        std::string label = "Carnivores: ";
                        std::string currentText = carnivoreText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        float value = 0.0f;
                        updateInputField(carnivoreInput, carnivoreText, label, currentText, value);
                        settings.carnivoreCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "plant") {
                        std::string label = "Plants: ";
                        std::string currentText = plantText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        float value = 0.0f;
                        updateInputField(plantInput, plantText, label, currentText, value);
                        settings.plantCount = static_cast<int>(value);
                    }
                    else if (activeInputField == "speed") {
                        std::string label = "Simulation Speed: ";
                        std::string currentText = speedText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        updateInputField(speedInput, speedText, label, currentText, settings.simulationSpeed);
                    }
                    else if (activeInputField == "growth") {
                        std::string label = "Plant Growth: ";
                        std::string currentText = growthText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        updateInputField(growthInput, growthText, label, currentText, settings.plantGrowthSpeed);
                    }
                    else if (activeInputField == "grid") {
                        std::string label = "Grid Size: ";
                        std::string currentText = gridSizeText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        float value = 0.0f;
                        updateInputField(gridSizeInput, gridSizeText, label, currentText, value);
                        settings.gridSize = static_cast<int>(value);
                    }
                    else if (activeInputField == "tile") {
                        std::string label = "Tile Size: ";
                        std::string currentText = tileSizeText.getString().toAnsiString();
                        if (currentText.length() > label.length()) {
                            currentText = currentText.substr(label.length());
                        } else {
                            currentText = "";
                        }
                        currentText += input;
                        float value = 0.0f;
                        updateInputField(tileSizeInput, tileSizeText, label, currentText, value);
                        settings.tileSize = static_cast<int>(value);
                    }
                }
            }
        }
    }
}

void UI::draw() {
    window.clear(sf::Color(50, 50, 50));
    
    // Draw input fields with highlighted active field
    window.draw(herbivoreInput);
    window.draw(carnivoreInput);
    window.draw(plantInput);
    window.draw(speedInput);
    window.draw(growthInput);
    window.draw(gridSizeInput);
    window.draw(tileSizeInput);
    
    // Highlight active input field
    if (activeInputField == "herbivore") {
        sf::RectangleShape highlight = herbivoreInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "carnivore") {
        sf::RectangleShape highlight = carnivoreInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "plant") {
        sf::RectangleShape highlight = plantInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "speed") {
        sf::RectangleShape highlight = speedInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "growth") {
        sf::RectangleShape highlight = growthInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "grid") {
        sf::RectangleShape highlight = gridSizeInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    } else if (activeInputField == "tile") {
        sf::RectangleShape highlight = tileSizeInput;
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(3.0f);
        window.draw(highlight);
    }
    
    // Draw text labels
    window.draw(herbivoreText);
    window.draw(carnivoreText);
    window.draw(plantText);
    window.draw(speedText);
    window.draw(growthText);
    window.draw(gridSizeText);
    window.draw(tileSizeText);
    
    // Draw start button
    window.draw(startButton);
    window.draw(startButtonText);
    
    window.display();
}

bool UI::run() {
    while (window.isOpen() && !isStarted) {
        handleEvents();
        draw();
    }
    return isStarted;
}

UI::SimulationSettings UI::getSettings() const {
    return settings;
}