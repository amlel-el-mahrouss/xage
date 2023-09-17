#pragma once

#if defined(__NPLICIT_DLL__)
#ifdef _WIN32
#ifdef __EXPORT_NPLICIT__
#	define NPLICIT_API __declspec(dllexport)
#else
#	define NPLICIT_API __declspec(dllimport)
#endif
#else
#	define NPLICIT_API
#endif
#else
#	define NPLICIT_API
#endif // ifdef __XPLICIT_DLL__

#ifndef __XPLICIT_DLL__

#include <functional>
#include <NginCore.h>
#include <Avx.h>

#endif

#define NPLICIT_RELEASE "1.0.1-NewReno"
#define NPLICIT_SPLASH_SCREEN printf("NplicitNgin - (c) XPX Corporation, all rights reserved.\n")

namespace XPX
{

}