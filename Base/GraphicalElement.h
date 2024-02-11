#ifndef GRAPHICALELEMENT_H
#define GRAPHICALELEMENT_H

#include <string>
#include <mutex>
#include <atomic>
//really basic abstraction of graphical elements
//main methods (Init, Render, Deinit) are thread-safe, other might be made by using 'm_lock'
class GraphicalElement
{
public:
	GraphicalElement();
	void Init(int id);
	virtual void SetName(std::string title);
	void Render();
	void Deinit();
	int GetID();
	std::string GetName();
	void SetAutoResize(bool state);
	int GetWindowWidth();
	int GetWindowHeight();
	//configuration save/load
	virtual std::string GetConfig() = 0;
	virtual void SetConfig(std::string& config) = 0;
	virtual ~GraphicalElement();
protected:
	virtual void InitInternal() = 0;
	virtual void DeinitInternal() = 0;
	virtual void RenderInternal() = 0;
	
	std::mutex m_lock;
	int m_id;
	std::string m_title;
	std::atomic_bool m_internalInitialized;

	int m_contextWidth;
	int m_contextHeight;

	bool m_autoSize;
};

#endif //GRAPHICALELEMENT_H