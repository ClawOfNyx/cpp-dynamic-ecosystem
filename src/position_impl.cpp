#include "position_impl.h"
#include <cmath>

using namespace std;

int PositionImpl::distanceToPoint(PositionImpl other){
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
