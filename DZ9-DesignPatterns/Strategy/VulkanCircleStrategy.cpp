#include "VulkanCircleStrategy.h"
#include <iostream>

void VulkanCircleStrategy::operator()(Circle const& circle) const
{
	std::cout << "Lik: Circle" << std::endl
		<< "Koristena biblioteka za crtanje: Vulkan" << std::endl << std::endl;
}