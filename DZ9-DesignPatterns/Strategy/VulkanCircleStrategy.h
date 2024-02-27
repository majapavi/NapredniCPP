#pragma once
#include "circle.h"

class VulkanCircleStrategy
{
public:
    void operator()(Circle const& circle) const;
};