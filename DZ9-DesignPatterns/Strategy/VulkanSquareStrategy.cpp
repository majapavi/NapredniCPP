#include "VulkanSquareStrategy.h"
#include <iostream>

void VulkanSquareStrategy::operator()(Square const& square) const
{
	std::cout << "Lik: Square" << std::endl
		<< "Koristena biblioteka za crtanje: Vulkan" << std::endl << std::endl;
}