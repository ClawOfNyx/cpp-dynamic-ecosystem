#ifndef GRID_IMPL_H
#define GRID_IMPL_H
#include <vector>
#include "Tile.h"

class GridImpl {
private:
    int width;
    int height;
    std::vector<std::vector<Tile>> tiles;

public:
    GridImpl(int width, int height);
    ~GridImpl();

    Tile& getTile(int x, int y);
    void setTile(int x, int y, const Tile& tile);
    Tile& findClosestEmptyTile(const Position& pos);
    Organism& findClosestOrganism(const Position& pos, OrganismType targetType) const;
    bool isInBounds(int x, int y) const;
};

#endif