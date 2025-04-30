#include "position.h"
#include "position_impl.h"

using namespace std;

Position::Position(int x, int y){
    pImpl = new PositionImpl(x, y);
}

Position::~Position(){
    delete pImpl;
}

int Position::getX() const { return pImpl->getX(); }
int Position::getY() const { return pImpl->getY(); }

void Position::setX(int X) { pImpl->setX(X); }
void Position::setY(int Y) { pImpl->setY(Y); }

int Position::distanceToPoint(const Position& other) const { return pImpl->distanceToPoint(*other.pImpl); }

vector<Position> Position::getAdjacentPositions() const {
    vector<PositionImpl> impls = pImpl->getAdjacentPositions();
    vector<Position> positions;
    positions.reserve(impls.size());
    for (const auto& impl : impls) {
        positions.emplace_back(impl.getX(), impl.getY());
    }
    return positions;
}