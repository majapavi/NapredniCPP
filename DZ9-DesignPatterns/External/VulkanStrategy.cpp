#include "VulkanStrategy.h"
#include <iostream>

void VulkanStrategy::operator()(const Square& square) const
{
	std::cout << "Lik: Square" << std::endl
		<< "Koristena biblioteka za crtanje: Vulkan" << std::endl << std::endl;
}

void VulkanStrategy::operator()(const Circle& square) const
{
	std::cout << "Lik: Circle" << std::endl
		<< "Koristena biblioteka za crtanje: Vulkan" << std::endl << std::endl;
}