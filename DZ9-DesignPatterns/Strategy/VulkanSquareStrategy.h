#pragma once
#include "square.h"

class VulkanSquareStrategy
{
public:
    void operator()(Square const& square) const;
};