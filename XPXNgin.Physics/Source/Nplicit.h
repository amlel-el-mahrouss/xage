/*
 * =====================================================================
 *
 *			NplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Config.h"
#include "NMath.h" /* Nplicit Math Library */

namespace XPX
{
#ifndef __NPLICIT_DETAIL_IEE754__
	typedef float NplicitFloat;
#else
	typedef double NplicitFloat;
#endif
}
