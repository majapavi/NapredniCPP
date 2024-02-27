#pragma once
#include "circle.h"

class OpenGLCircleStrategy
{
public:
    void operator()(Circle const& circle) const;
};