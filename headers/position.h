#ifndef POSITION_H
#define POSITION_H
#include <vector>

class PositionImpl;

class Position {
private:
    PositionImpl* pImpl;
public:
    Position(int x, int y);
    
    Position(const Position& other);
    Position& operator=(const Position& other);
    
    ~Position();

    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);
    int distanceToPoint(const Position& other) const;
    std::vector<Position> getAdjacentPositions() const;
};

#endif