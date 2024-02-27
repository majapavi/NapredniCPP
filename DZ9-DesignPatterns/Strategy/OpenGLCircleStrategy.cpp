#include "OpenGLCircleStrategy.h"
#include <iostream>

void OpenGLCircleStrategy::operator()(Circle const& circle) const
{
	std::cout << "Lik: Circle" << std::endl
		<< "Koristena biblioteka za crtanje: OpenGL" << std::endl << std::endl;
}