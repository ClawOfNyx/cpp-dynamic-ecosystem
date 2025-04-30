#include "grid_impl.h"
#include <stdexcept>
#include <cmath>

using namespace std;

GridImpl::GridImpl(int width, int height) : width(width), height(height) {
    tiles.resize(height, vector<Tile>(width, Tile(Position(0, 0))));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            tiles[y][x] = Tile(Position(x, y));
        }
    }
}

GridImpl::~GridImpl() {}

Tile& GridImpl::getTile(int x, int y) {
    if (!isInBounds(x, y)) {
        throw out_of_range("Coordinates are out of bounds.");
    }
    return tiles[y][x];
}

void GridImpl::setTile(int x, int y, const Tile& tile) {
    if (!isInBounds(x, y)) {
        throw out_of_range("Coordinates are out of bounds.");
    }
    tiles[y][x] = tile;
}

Tile& GridImpl::findClosestEmptyTile(const Position& pos) {
    Tile* closestTile = nullptr;
    double closestDistance = std::numeric_limits<double>::max();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Tile& currentTile = getTile(x, y);
            
            if (currentTile.isEmpty()) {
                double dx = pos.getX() - x;
                double dy = pos.getY() - y;
                double distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestTile = &currentTile;
                }
            }
        }
    }
    
    if (closestTile == nullptr) {
        throw std::runtime_error("No empty tiles found in the grid");
    }
    
    return *closestTile;
}

Organism& GridImpl::findClosestOrganism(const Position& pos, OrganismType targetType) const {
    Organism* closestOrganism = nullptr;
    double closestDistance = std::numeric_limits<double>::max();
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!tiles[y][x].isEmpty() && tiles[y][x].getOccupant().getType() == targetType) {
                double dx = pos.getX() - x;
                double dy = pos.getY() - y;
                double distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestOrganism = &tiles[y][x].getOccupant();
                }
            }
        }
    }
    
    if (closestOrganism == nullptr) {
        throw std::runtime_error("No organism of the specified type found.");
    }
    
    return *closestOrganism;
}

bool GridImpl::isInBounds(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}
