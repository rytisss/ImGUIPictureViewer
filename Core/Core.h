#ifndef CORE_H
#define CORE_H

#if defined (_WIN32) 
#if defined(CORE_EXPORT)
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif /* CORE_API */
#define _sprintf sprintf_s
#endif

#if defined(__GNUC__)
//  GCC
#define CORE_API __attribute__((visibility("default")))
#endif


#endif //CORE_H