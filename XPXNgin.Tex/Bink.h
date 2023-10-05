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

#define BINK_VIDEO_FORMAT ".bink"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

//! to send to directx.
typedef void* BinkData;

#define XAGE_BINK_MALLOC malloc
#define XAGE_BINK_FREE free

inline BinkHeader* BinkReadContainer(FILE* file)
{
	BinkHeader* readHdr = XAGE_BINK_MALLOC(sizeof(BinkHeader));

	if (fread(readHdr, sizeof(BinkHeader), SEEK_SET, file) == sizeof(BinkHeader))
		return readHdr;

	XAGE_BINK_FREE(readHdr);
	return NULL;
}

inline BOOL BinkContainerCheck(BinkHeader* hdr)
{
	return strcmp(hdr->mag, XIFF_VIDEO_MAG);
}
