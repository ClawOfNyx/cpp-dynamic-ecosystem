#ifndef POSITION_IMPL_H
#define POSITION_IMPL_H
#include <vector>
#include <iostream>

class PositionImpl {
private:
    int x, y;
public:
    PositionImpl(int X, int Y);
    ~PositionImpl();

    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int X) { x = X; }
    void setY(int Y) { y = Y; }

    int distanceToPoint(const PositionImpl& other);
    vector<PositionImpl> getAdjacentPositions();
};
#endif