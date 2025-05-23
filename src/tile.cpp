#include "Tile.h"
#include "TileImpl.h"
#include <stdexcept>

using namespace std;

Tile::Tile(const Position& position) {
    pImpl = new TileImpl(position);
}
Tile::~Tile() {
    delete pImpl;
}
bool Tile::isEmpty() const {
    return pImpl->isEmpty();
}
Organism* Tile::getOccupant() const {
    return pImpl->getOccupant();
}
void Tile::setOccupant(const Organism& organism) {
    pImpl->setOccupant(organism);
}
void Tile::clearOccupant() {
    pImpl->clearOccupant();
}

Position& Tile::getPosition() {
    return pImpl->getPosition();
}
