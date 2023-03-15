/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Avx.j
 *			Purpose: Intel AVX based C functions.
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

#ifdef XPLICIT_WINDOWS

#include <immintrin.h>

XPLICIT_API bool avx_strcmp(const char* str1, const char* str2);

// use XPLICIT_USE_AVX, in order to tell if you want AVX or not.

#ifdef XPLICIT_WANT_AVX
#define XPLICIT_USE_AVX (1)
#endif // idef XPLICIT_WANT_AVX

#endif