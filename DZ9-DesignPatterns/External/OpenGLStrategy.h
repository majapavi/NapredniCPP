#pragma once
#include "circle.h"
#include "square.h"

class OpenGLStrategy
{
public:
    void operator()(Circle const& circle) const;
    void operator()(Square const& square) const;
};