#ifndef TILE_H
#define TILE_H
#include "Position.h"
#include "Organism.h"

class TileImpl;

class Tile {
private:
    TileImpl* pImpl;
public:
    Tile(const Position& position);
    ~Tile();

    bool isEmpty() const;
    Organism& getOccupant() const;
    void setOccupant(const Organism& organism);
    void clearOccupant();
    Position& getPosition();
};

#endif