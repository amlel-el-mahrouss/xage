/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: Avx.h
 *			Purpose: Intel AVX Library
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifdef XPLICIT_WINDOWS

#include <immintrin.h>

XPLICIT_API bool avx_strequals(const char* str1, const char* str2);

// use XPLICIT_USE_VECTOR, in order to tell if you want AVX or not.

#ifdef XPLICIT_WANT_VECTOR
#define XPLICIT_USE_VECTOR (1)
#endif // ifdef XPLICIT_WANT_AVX

#endif