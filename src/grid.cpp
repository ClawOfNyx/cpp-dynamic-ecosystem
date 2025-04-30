#include "grid.h"
#include <stdexcept>

using namespace std;

Grid::Grid(int width, int height) : pImpl(new GridImpl(width, height)) {}

Grid::~Grid() {
    delete pImpl;
}

Tile& Grid::getTile(int x, int y) const {
    return pImpl->getTile(x, y);
}

void Grid::setTile(int x, int y, const Tile& tile) {
    pImpl->setTile(x, y, tile);
}

Tile& Grid::findClosestEmptyTile(const Position& pos) const {
    return pImpl->findClosestEmptyTile(pos);
}

Organism& Grid::findClosestOrganism(const Position& pos, OrganismType targetType) const {
    return pImpl->findClosestOrganism(pos, targetType);
}

bool Grid::isInBounds(int x, int y) const {
    return pImpl->isInBounds(x, y);
}