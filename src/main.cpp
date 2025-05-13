#include <iostream>
#include <string>
#include <cstdlib> // For rand()
#include "WorldManager.h"
#include "Animal.h"
#include "Plant.h"
#include "Position.h"

using namespace std;

int main() {
    try {
        cout << "Starting debug program...\n";
        
        cout << "Creating Position object...\n";
        Position pos(5, 5);
        cout << "Position created at (" << pos.getX() << ", " << pos.getY() << ")\n";
        
        cout << "Creating Plant object...\n";
        Plant* plant = new Plant(5.0f, 100, 0.5f, 0.3f);
        cout << "Plant created with nutrients: " << plant->getNutrients() << "\n";
        delete plant;
        
        cout << "Creating Animal object...\n";
        Animal* animal = new Animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 15.0f, 5);
        cout << "Animal created with nutrients: " << animal->getNutrients() << "\n";
        delete animal;
        
        cout << "Creating world (20x20)...\n";
        WorldManager world(20, 20, 1.0f);
        cout << "World created successfully\n";
        
        cout << "Press Enter to exit...";
        cin.get();
        
        return 0;
    } catch (const std::exception& e) {
        cerr << "ERROR: " << e.what() << "\n";
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred!\n";
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    }
}