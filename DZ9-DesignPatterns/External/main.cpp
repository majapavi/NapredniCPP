#include "circle.h"
#include "square.h"
#include "shape.h"
#include "VulkanStrategy.h"
#include "OpenGLStrategy.h"

#include <vector>
#include <iostream>
//#include <memory>

int main()
{
    VulkanStrategy strategy1{};
    OpenGLStrategy strategy2{};

    auto c1 = new ShapeModel<Circle>(Circle(1, { 2,3 }), strategy1);
    auto c2 = new ShapeModel<Circle>(Circle(2, { 0,1 }), strategy2);
    auto s1 = new ShapeModel<Square>(Square(1, { 0,0 }), strategy1);
    auto s2 = new ShapeModel<Square>(Square(3, { 2,3 }), strategy2);

    std::vector<ShapeConcept*> shapes;
    shapes.push_back(c1); shapes.push_back(s1); shapes.push_back(c2); shapes.push_back(s2);

    std::cout << "External polymorphism design pattern" << std::endl << std::endl;
    for (ShapeConcept* s : shapes) s->draw();

    delete s2; delete s1; delete c2; delete c1;

    return 0;
}