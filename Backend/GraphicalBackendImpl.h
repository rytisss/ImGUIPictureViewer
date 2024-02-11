#ifndef GRAPHICALBACKENDIMPL_H
#define GRAPHICALBACKENDIMPL_H

#include "GraphicalBackend.h"
#include "ContextWindow.h"

class GraphicalBackend::GraphicalBackendImpl
{
public:
	//constructor
	GraphicalBackendImpl(GraphicalBackend& parentRef);
	//run app
	void Enter();
	//destructor
	~GraphicalBackendImpl();
private:
	ContextWindow m_gui;
};

#endif //GRAPHICALBACKENDIMPL_H