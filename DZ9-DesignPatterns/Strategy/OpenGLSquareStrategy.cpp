#include "OpenGLSquareStrategy.h"
#include <iostream>

void OpenGLSquareStrategy::operator()(const Square& square) const
{
	std::cout << "Lik: Square" << std::endl
		<< "Koristena biblioteka za crtanje: OpenGL" << std::endl << std::endl;
}