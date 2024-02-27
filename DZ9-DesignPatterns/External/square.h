#pragma once
#include <utility>
#include "point.h"

class Square
{
public:
    Square(float side, Point ulc) : side_{ side }, upper_left_corner_{ ulc } {}

    float side() const { return side_; }
    Point upper_left_corner() const { return upper_left_corner_; }

private:
    float side_;
    Point upper_left_corner_;
};
