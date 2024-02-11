#ifndef GRAPHICALBACKEND_H
#define GRAPHICALBACKEND_H

#if defined (_WIN32) 
#if defined(BACKEND_EXPORT)
#define BACKEND_API __declspec(dllexport)
#else
#define BACKEND_API __declspec(dllimport)
#endif /* BACKEND_API */
#define _sprintf sprintf_s
#endif

#if defined(__GNUC__)
//  GCC
#define BACKEND_API __attribute__((visibility("default")))
#endif

class BACKEND_API GraphicalBackend
{
public:
	GraphicalBackend();
	// Start app
	void Enter();
	~GraphicalBackend();
public: 
	class GraphicalBackendImpl;
	GraphicalBackendImpl* pGraphicalRunnerImpl;
};

#endif //GRAPHICALRUNNER_H