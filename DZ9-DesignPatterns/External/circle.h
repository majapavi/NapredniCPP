#pragma once
#include <utility>
#include "point.h"

class Circle
{
public:
    Circle(float r, Point center) : radius_{ r }, center_{ center } {}

    float radius() const { return radius_; }
    Point center() const { return center_; }

private:
    float radius_;
    Point  center_;
};
