#include "GraphicalElement.h"
#include <iostream>

GraphicalElement::GraphicalElement()
	: m_id(-1)
	, m_title("no_name")
	, m_internalInitialized(false)
	, m_contextWidth(0)
	, m_contextHeight(0)
	, m_autoSize(false)
{
}

void GraphicalElement::Init(int id)
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_id = id;
	InitInternal();
	m_internalInitialized = true;
}

void GraphicalElement::SetName(std::string title)
{
	m_title = title;
}

void GraphicalElement::Render()
{
	std::unique_lock<std::mutex> lock(m_lock);
	RenderInternal();
}

void GraphicalElement::Deinit()
{
	if (m_internalInitialized)
	{
		std::unique_lock<std::mutex> lock(m_lock);
		std::cout << "Deinitializing graphical element " << std::to_string(m_id) << " with name " << "\'" << m_title << "\'" << std::endl;
		DeinitInternal();
		m_internalInitialized = false;
	}
}

int GraphicalElement::GetID()
{
	return m_id;
}

std::string GraphicalElement::GetName()
{
	return m_title;
}

void GraphicalElement::SetAutoResize(bool state)
{
	m_autoSize = state;
}

int GraphicalElement::GetWindowWidth()
{
	return m_contextWidth;
}

int GraphicalElement::GetWindowHeight()
{
	return m_contextHeight;
}

GraphicalElement::~GraphicalElement()
{
	Deinit();
}
