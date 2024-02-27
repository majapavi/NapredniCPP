#include "circle.h"
#include "square.h"
#include "VulkanCircleStrategy.h"
#include "VulkanSquareStrategy.h"
#include "OpenGLCircleStrategy.h"
#include "OpenGLSquareStrategy.h"

#include <vector>
#include <iostream>
//#include <memory>

int main()
{
    Circle* c1 = new Circle(1, { 2,3 }, OpenGLCircleStrategy{});
    Circle* c2 = new Circle(2, { 0,1 }, VulkanCircleStrategy{});
    Square* s1 = new Square(1, { 2,3 }, OpenGLSquareStrategy{});
    Square* s2 = new Square(2, { 0,1 }, VulkanSquareStrategy{});

    std::vector<Shape*> shapes;
    shapes.push_back(c1); shapes.push_back(s1); shapes.push_back(c2); shapes.push_back(s2);

    std::cout << "Strategy design pattern" << std::endl << std::endl;
    for (Shape* s : shapes) s->draw();

    delete s2; delete s1; delete c2; delete c1;

    return 0;
}