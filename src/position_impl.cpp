#include "position_impl.h"
#include <cmath>

using namespace std;

PositionImpl::PositionImpl(int X, int Y){
    if (X < 0 || Y < 0) {
        throw invalid_argument("Coordinates cannot be negative.");
    }
    else {
        x = X;
        y = Y;
    }
}
PositionImpl::~PositionImpl() {}

int PositionImpl::distanceToPoint(const PositionImpl& other){
    return static_cast<int>(sqrt(pow(other.getX() - x, 2) + pow(other.getY() - y, 2)));
}

vector<PositionImpl> PositionImpl::getAdjacentPositions(){
    return vector<PositionImpl>{
        PositionImpl(x - 1, y),
        PositionImpl(x + 1, y),
        PositionImpl(x, y - 1),
        PositionImpl(x, y + 1),
        PositionImpl(x - 1, y - 1),
        PositionImpl(x - 1, y + 1),
        PositionImpl(x + 1, y + 1),
        PositionImpl(x + 1, y - 1)
    };
}
