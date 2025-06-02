#include "TileImpl.h"
#include <stdexcept>
#include <iostream>

using namespace std;

TileImpl::TileImpl(const Position& pos) : position(pos), organism(nullptr) {
    //cout << "Creating Tile at position (" << pos.getX() << ", " << pos.getY() << ")" << endl;
}  

TileImpl::~TileImpl() {}

bool TileImpl::isEmpty() const {
    return organism == nullptr;
}

Organism* TileImpl::getOccupant() const {
    return organism;
}

void TileImpl::setOccupant(const Organism& org) {
    if (organism != nullptr) {
        cerr << "Error: Tile at (" << position.getX() << ", " << position.getY() 
             << ") already has an occupant. Cannot place new organism." << endl;
        throw std::runtime_error("Attempt to place organism on occupied tile");
    }
    organism = const_cast<Organism*>(&org);
    cout << "Organism placed at (" << position.getX() << ", " << position.getY() << ")" << endl;
}

void TileImpl::clearOccupant() {
    organism = nullptr;
}

Position& TileImpl::getPosition() {
    return position;
}