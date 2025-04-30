#include "tile_impl.h"
#include <stdexcept>

using namespace std;

TileImpl::TileImpl(const Position& pos) : position(pos), organism(nullptr) {}  

TileImpl::~TileImpl() {}

bool TileImpl::isEmpty() const {
    return organism == nullptr;
}

Organism& TileImpl::getOccupant() const {
    if (isEmpty()) {
        throw runtime_error("Tile is empty, no occupant to return.");
    }
    return *organism;
}

void TileImpl::setOccupant(const Organism& org) {
    if (organism != nullptr) {
        throw runtime_error("Tile already has an occupant.");
    }
    //organism = new Organism(org);
}
void TileImpl::clearOccupant() {
    if (organism != nullptr) {
        organism = nullptr;
    }
}

    