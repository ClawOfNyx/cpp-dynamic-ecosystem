#include <iostream>
#include <string>
#include <cstdlib> // For rand()
#include "WorldManager.h"
#include "Animal.h"
#include "Plant.h"
#include "Position.h"

using namespace std;

int main() {
        cout << "Starting debug program" << endl;
        
        cout << "Creating world (20x20)" << endl;
        WorldManager& world = WorldManager::getInstance(20, 20, 1.0f);
        cout << "World created successfully" << endl;
        
        cout << "Testing grid access" << endl;
        
        cout << "Creating Plant object\n";
        Plant* plant = new Plant(5.0f, 100, 0.5f, 0.3f);
        cout << "Plant created with nutrients: " << plant->getNutrients() << "\n";
        delete plant;
        
        cout << "Creating Animal object\n";
        Animal* animal = new Animal(10.0f, 80, 2, 5, AnimalType::HERBIVORE, 1.0f, 15.0f, 5);
        cout << "Animal created with nutrients: " << animal->getNutrients() << "\n";
        delete animal;
        
        cout << "Press Enter to exit...";
        //cin.ignore();
        cin.get();
        
        return 0;

}