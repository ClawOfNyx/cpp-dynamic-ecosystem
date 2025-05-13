#ifndef GRID_H
#define GRID_H
#include <vector>
#include "GridImpl.h"

class Grid{
private:
    GridImpl* pImpl;
public:
    Grid(int width, int height);
    ~Grid();

    Tile& getTile(int x, int y) const;
    void setTile(int x, int y, const Tile& tile);
    Tile& findClosestEmptyTile(const Position& pos) const;
    Organism& findClosestOrganism(const Position& pos, OrganismType targetType) const;
    bool isInBounds(int x, int y) const;
};

#endif