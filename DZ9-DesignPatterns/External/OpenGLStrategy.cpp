#include "OpenGLStrategy.h"
#include <iostream>

void OpenGLStrategy::operator()(const Square& square) const
{
	std::cout << "Lik: Square" << std::endl
		<< "Koristena biblioteka za crtanje: OpenGL" << std::endl << std::endl;
}

void OpenGLStrategy::operator()(const Circle& square) const
{
	std::cout << "Lik: Circle" << std::endl
		<< "Koristena biblioteka za crtanje: OpenGL" << std::endl << std::endl;
}