#include "PositionImpl.h"
#include <cmath>

using namespace std;

PositionImpl::PositionImpl(int X, int Y) {
    // Remove the negative coordinate check since we might need to handle them
    x = X;
    y = Y;
}

PositionImpl::~PositionImpl() {}

int PositionImpl::distanceToPoint(const PositionImpl& other){
    return static_cast<int>(sqrt(pow(other.getX() - x, 2) + pow(other.getY() - y, 2)));
}

std::vector<PositionImpl> PositionImpl::getAdjacentPositions(){
    std::vector<PositionImpl> validPositions;
    
    // All 8 possible adjacent positions
    std::vector<std::pair<int, int>> offsets = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };
    
    for (const auto& offset : offsets) {
        int newX = x + offset.first;
        int newY = y + offset.second;
        
        // Only add positions that are non-negative
        if (newX >= 0 && newY >= 0) {
            validPositions.emplace_back(newX, newY);
        }
    }
    
    return validPositions;
}