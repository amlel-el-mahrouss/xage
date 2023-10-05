/*
 * =====================================================================
 *
 *			XAGE.Tex
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: XIFF container header.
 *
 * =====================================================================
 */

#pragma once

#include "XIFF.h"

//! XAGE Bink is based on XIFF. A Format based on IFF.
//! IFF was made by EA back for the amiga.
//! We use this for cinematics and video stuff.

typedef struct XiffVideoHeader BinkHeader;

#include <stdio.h>

//! to send to directx.
typedef void* BinkData;

