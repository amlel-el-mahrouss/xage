#pragma once

#ifndef _WIN32
#	error The NplicitNgin only works for Windows
#endif

#ifdef __NPLICIT_DLL__
#ifdef __EXPORT_NPLICIT__
#	define NPLICIT_API __declspec(dllexport)
#else
#	define NPLICIT_API __declspec(dllimport)
#endif
#else
#	define NPLICIT_API
#endif // ifdef __XPLICIT_DLL__

#ifndef __XPLICIT_DLL__

#include <functional>
#include <Xplicit.h>
#include <Avx.h>

#endif

#define NPLICIT_RELEASE "1.0.0-Reno"
#define NPLICIT_SPLASH_SCREEN printf("NplicitNgin - (c) XPX Corporation, all rights reserved.")

namespace XPX
{

}