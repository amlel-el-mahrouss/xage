/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#ifdef _WIN32
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN

#   define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#endif // ifdef _WIN32

#include <ctime>
#include <cstdio>
#include <clocale>
#include <cstring>
#include <codecvt>
#include <cassert>
#include <cstdlib>
#include <cstdint>

#include <spdlog/spdlog.h>
#include <uuid/uuid.h>

#include <WinSock2.h>
#include <ws2tcpip.h>

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

#ifdef _WIN32

#include <Windows.h>
#include <tlhelp32.h>

#include <shellapi.h>
#include <CommCtrl.h>

#pragma comment(lib,"comctl32.lib")
#pragma comment(lib, "Ws2_32.lib")

#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef _WIN32
#define XPLICIT_WINDOWS 1
#endif // ifdef _MSC_VER

#ifdef __EXPORT_XPLICIT__
#   define XPLICIT_API __declspec(dllexport)
#else
#   define XPLICIT_API __declspec(dllimport)
#endif


#ifdef _MSC_VER
#   define XPLICIT_MSVC (1)
#   define XPLICIT_CXX_COMPILER "Microsoft Visual C++"
#   define XPLICIT_CXX (1)
#endif // ifdef _MSC_VER

#endif

#ifndef _NDEBUG
#   define XPLICIT_DEBUG (1)
#else
#   define XPLICIT_RELEASE (2)
#endif

#ifdef XPLICIT_DEBUG
#   define XPLICIT_ASSERT(expression) (void)(                                                \
            (!!(expression)) ||                                                              \
            (_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
        )
#else
#   define XPLICIT_ASSERT(expression)
#endif

#ifndef XPLICIT_ENV
#   define XPLICIT_ENV getenv
#endif // XPLICIT_ENV

#define XPLICIT_COPY_DELETE(KLASS)\
KLASS& operator=(const KLASS&) = delete;\
KLASS(const KLASS&) = delete;\



#define XPLICIT_COPY_DEFAULT(KLASS)\
KLASS& operator=(const KLASS&) = default;\
KLASS(const KLASS&) = default;\



#define XPLICIT_MOVE_DELETE(KLASS)\
KLASS& operator=(KLASS&&) = delete;\
KLASS(KLASS&&) = delete;\



#define XPLICIT_MOVE_DEFAULT(KLASS)\
KLASS& operator=(KLASS&&) = default;\
KLASS(KLASS&&) = default;\

#ifndef XPLICIT_SLEEP
#   ifdef XPLICIT_WINDOWS
#   define XPLICIT_SLEEP Sleep
#   else
#   error You need a sleep function
#   endif
#endif /* ifndef */

namespace Xplicit
{
    typedef double Double;
    typedef float Real;

    typedef std::int32_t Int32;
    typedef std::uint32_t UInt32;
    
    typedef std::int64_t Int64;
    typedef std::uint64_t UInt64;
}