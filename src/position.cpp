#include "Position.h"
#include "PositionImpl.h"

using namespace std;

Position::Position(int x, int y){
    pImpl = new PositionImpl(x, y);
}

Position::Position(const Position& other) {
    pImpl = new PositionImpl(other.pImpl->getX(), other.pImpl->getY());
}

Position& Position::operator=(const Position& other) {
    if (this != &other) {
        delete pImpl;
        pImpl = new PositionImpl(other.pImpl->getX(), other.pImpl->getY());
    }
    return *this;
}

Position::~Position(){
    delete pImpl;
}

int Position::getX() const { return pImpl->getX(); }
int Position::getY() const { return pImpl->getY(); }

void Position::setX(int X) { pImpl->setX(X); }
void Position::setY(int Y) { pImpl->setY(Y); }

int Position::distanceToPoint(const Position& other) const { return pImpl->distanceToPoint(*other.pImpl); }

std::vector<Position> Position::getAdjacentPositions() const {
    std::vector<PositionImpl> impls = pImpl->getAdjacentPositions();
    std::vector<Position> positions;
    positions.reserve(impls.size());
    for (const auto& impl : impls) {
        positions.emplace_back(impl.getX(), impl.getY());
    }
    return positions;
}