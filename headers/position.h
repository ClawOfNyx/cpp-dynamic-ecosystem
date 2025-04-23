#ifndef POSITION_H
#define POSITION_H
#include <vector>

using namespace std;

class PositionImpl;

class Position {
private:
    PositionImpl* pImpl;
public:
    Position(int x, int y);
    ~Position();

    int getX() const;
    int getY() const;

    void setX(int x);
    void setY(int y);

    int distanceToPoint(Position other) const;
    vector<Position> getAdjacentPositions() const;
};

#endif