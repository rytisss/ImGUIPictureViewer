#ifndef PICTUREVIEWER_H
#define PICTUREVIEWER_H

#if defined (_WIN32) 
#if defined(PICTUREVIEWER_EXPORT)
#define PICTUREVIEWER_API __declspec(dllexport)
#else
#define PICTUREVIEWER_API __declspec(dllimport)
#endif /* PICTUREVIEWER_API */
#define _sprintf sprintf_s
#endif

#if defined(__GNUC__)
//  GCC
#define PICTUREVIEWER_API __attribute__((visibility("default")))
#endif

#endif // PICTUREVIEWER_H