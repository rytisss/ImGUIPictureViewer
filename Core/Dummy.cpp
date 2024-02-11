#include "Dummy.h"
#include <iostream>

Dummy::Dummy()
{
	std::cout << "Dummy initialized!" << std::endl;
}

Dummy::~Dummy()
= default;
