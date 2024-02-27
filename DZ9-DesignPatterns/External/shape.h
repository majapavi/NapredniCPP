#pragma once
#include <functional>

class ShapeConcept
{
public:
    virtual ~ShapeConcept() = default;
    virtual void draw() const = 0;
};

template <typename ShapeT>
class ShapeModel : public ShapeConcept
{
public:
    using DrawStrategy = std::function<void(ShapeT const&)>;

    ShapeModel(ShapeT shape, DrawStrategy drawer) :
        shape_{ std::move(shape) }, drawer_{ std::move(drawer) } {}

    virtual void draw() const override { drawer_(shape_); }

private:
    ShapeT shape_;
    DrawStrategy drawer_;
};