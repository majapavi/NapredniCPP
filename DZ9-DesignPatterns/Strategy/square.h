#pragma once
#include "shape.h"
#include <utility>
#include <functional>

class Square : public Shape
{
public:
    using DrawStrategy = std::function<void(Square const&)>;

    Square(float side, Point ulc, DrawStrategy drawer) :
        side_{ side }, upper_left_corner_{ ulc }, drawer_{ std::move(drawer) } {}

    float side() const { return side_; }
    Point upper_left_corner() const { return upper_left_corner_; }

    virtual void draw() const override { drawer_(*this); }

private:
    float side_;
    Point upper_left_corner_;
    DrawStrategy drawer_;
};