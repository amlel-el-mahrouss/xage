/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Config.h
 *			Purpose: Engine Config File
 *
 * =====================================================================
 */

#pragma once

#include <ctime>
#include <cstdio>
#include <clocale>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdint>

#include <uuid/uuid.h>

#ifdef _WIN32
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#endif

#include <spdlog/spdlog.h>

#include <WinSock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <tuple>
#include <array>
#include <thread>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <concepts>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

#include <Windows.h>
#include <tlhelp32.h>
#include <shellapi.h>

#include <CommCtrl.h>

#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _WIN32
#define XPLICIT_WINDOWS 1
#endif // ifdef _MSC_VER

#ifdef __XPLICIT_DLL__
#ifdef __EXPORT_XPLICIT__
#define XPLICIT_API __declspec(dllexport)
#else
#define XPLICIT_API __declspec(dllimport)
#endif
#else
#define XPLICIT_API
#endif // ifdef __XPLICIT_DLL__

#define XPLICIT_MSVC (1)
#define XPLICIT_GCC (2)

#ifdef _MSC_VER
#define XPLICIT_CXX_COMPILER "Visual C++"
#define XPLICIT_CXX (1)
#endif // ifdef _MSC_VER

#ifndef _NDEBUG
#define XPLICIT_DEBUG (1)
#else
#define XPLICIT_RELEASE (2)
#endif

#ifdef XPLICIT_DEBUG
#define XPLICIT_ASSERT(expression) (void)(                                                       \
            (!!(expression)) ||                                                              \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
        )
#else
#define XPLICIT_ASSERT(expression)
#endif

#ifndef XPLICIT_ENV
#define XPLICIT_ENV getenv
#endif // XPLICIT_ENV

#define XPLICIT_COPY_DELETE(KLASS)\
KLASS& operator=(const KLASS&) = delete;\
KLASS(const KLASS&) = delete;\



#define XPLICIT_COPY_DEFAULT(KLASS)\
KLASS& operator=(const KLASS&) = default;\
KLASS(const KLASS&) = default;\




#ifndef XPLICIT_SLEEP
#define XPLICIT_SLEEP Sleep
#endif
