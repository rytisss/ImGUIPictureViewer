#include "GraphicalBackend.h"
#include "GraphicalBackendImpl.h"
#include <iostream>

GraphicalBackend::GraphicalBackend()
	: pGraphicalRunnerImpl(new GraphicalBackend::GraphicalBackendImpl(*this))
{
}
void GraphicalBackend::Enter()
{
	std::cout << "Starting!" << std::endl;
	pGraphicalRunnerImpl->Enter();
}
GraphicalBackend::~GraphicalBackend()
{
	delete pGraphicalRunnerImpl;
}