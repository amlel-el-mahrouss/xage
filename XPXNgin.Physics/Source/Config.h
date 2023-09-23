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

#define NPLICIT_RELEASE "1.1.1-LasVegas"
#define NPLICIT_SPLASH_SCREEN printf("NplicitNgin - (c) XPX Corporation, all rights reserved.\nNplicitNgin 'Network Physics' is based on either NVIDIa(tm) PhysX or XPX own VRCL(tm) both of them are property of their respective owners.")
