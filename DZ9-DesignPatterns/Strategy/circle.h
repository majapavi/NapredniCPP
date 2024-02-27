#pragma once
#include "shape.h"
#include <utility>
#include <functional>

class Circle : public Shape
{
public:
    using DrawStrategy = std::function<void(Circle const&)>;

    Circle(float r, Point center, DrawStrategy drawer) :
        radius_{ r }, center_{ center }, drawer_{ std::move(drawer) } {}

    float radius() const { return radius_; }
    Point center() const { return center_; }

    virtual void draw() const override { drawer_(*this); }

private:
    float radius_;
    Point  center_;
    DrawStrategy drawer_;
};