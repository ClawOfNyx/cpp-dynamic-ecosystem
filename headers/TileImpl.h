#ifndef TILE_IMPL_H
#define TILE_IMPL_H
#include "Position.h"
#include "Organism.h"

class TileImpl {
private:
    Position position;
    Organism* organism;
public:
    TileImpl(const Position& pos);
    ~TileImpl();

    bool isEmpty() const;
    Organism& getOccupant() const;
    void setOccupant(const Organism& organism);
    void clearOccupant();
    Position& getPosition();
};

#endif