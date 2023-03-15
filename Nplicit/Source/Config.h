#pragma once

#ifndef _WIN32
#error The NplicitNgin only works for Windows
#endif

#ifdef __NPLICIT_DLL__
#ifdef __EXPORT_NPLICIT__
#define NPLICIT_API __declspec(dllexport)
#else
#define NPLICIT_API __declspec(dllimport)
#endif
#else
#define NPLICIT_API
#endif // ifdef __XPLICIT_DLL__
