#include "GraphicalBackendImpl.h"

GraphicalBackend::GraphicalBackendImpl::GraphicalBackendImpl(GraphicalBackend& parentRef)
{
}

void GraphicalBackend::GraphicalBackendImpl::Enter()
{
	m_gui.Init();
	m_gui.Run();
	m_gui.Deinit();
}

GraphicalBackend::GraphicalBackendImpl::~GraphicalBackendImpl()
{
}
