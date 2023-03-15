/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Avx.cpp
 *			Purpose: Intel AVX based C functions.
 *
 * =====================================================================
 */

#include "Avx.h"

#ifdef XPLICIT_WINDOWS

bool avx_strcmp(const char* str1, const char* str2)
{
	__m256i a = _mm256_load_si256((__m256i*)str2);
	__m256i b = _mm256_load_si256((__m256i*)str2);

	__m256i pcmp = _mm256_cmpeq_epi32(a, b);  // epi8 is fine too
	unsigned bitmask = _mm256_movemask_epi8(pcmp);

	return (bitmask == 0xffffffffU);
}

#endif