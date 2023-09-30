/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Avx.h
 *			Purpose: Intel AVX Library
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#ifdef XPLICIT_WINDOWS

#include <immintrin.h>

XPLICIT_API bool avx_strequals(const char* str1, const char* str2);

// use XPLICIT_USE_AVX, in order to tell if you want AVX or not.

#ifdef XPLICIT_WANT_AVX
#define XPLICIT_USE_AVX (1)
#endif // ifdef XPLICIT_WANT_AVX

#endif