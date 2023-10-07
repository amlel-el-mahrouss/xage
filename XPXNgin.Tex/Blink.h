/*
 * =====================================================================
 *
 *			XAGE.Tex
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: BLINK decoder.
 *
 * =====================================================================
 */

#pragma once

#include "XIFF.h"

//! XAGE Blink is based on XIFF. A Format based on IFF.
//! IFF was made by EA back for the amiga.
//! We use this for video stuff.

typedef struct XiffVideoHeader BlinkHeader;

#define BLINK_VIDEO_FORMAT ".vid"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

//! Send to DirectX..
typedef char BlinkData[4096];

#define XAGE_BLINK_MALLOC malloc
#define XAGE_BLINK_FREE free

static inline BlinkHeader* BlinkReadContainer(FILE* file)
{
	BlinkHeader* readHdr = XAGE_BLINK_MALLOC(sizeof(BlinkHeader));

	if (fread(readHdr, sizeof(BlinkHeader), SEEK_SET, file) == sizeof(BlinkHeader))
		return readHdr;

	XAGE_BLINK_FREE(readHdr);
	return NULL;
}

#define XIFF_FORMAT_BLINK (0x5775FF)

static inline BOOL BlinkContainerCheck(BlinkHeader* hdr)
{
	return strcmp(hdr->mag, XIFF_VIDEO_MAG) &&
		hdr->format_type = XIFF_FORMAT_BLINK;
}

struct BlinkPrivateData
{
	SIZE_T f_szData;
	WORD f_iFormat;
	WORD f_iType;
	BlinkData f_pData;
};

enum
{
	BLINK_DATA_TYPE_VIDEO,
	BLINK_DATA_TYPE_AUDIO,
};

enum
{
	BLINK_DATA_FORMAT_R32G32B32A32,
	BLINK_DATA_FORMAT_R16G16B16A16,
};

enum
{
	BLINK_DATA_VIEWPORT_1280x720,
	BLINK_DATA_VIEWPORT_800x600,
};

struct BlinkVideoFrame
{
	UINT f_iTexture;
	struct BlinkPrivateData f_pPrivateData;

	struct BlinkVideoFrame* f_pNext;
};

#include <assert.h>

#define BlinkZeroMemoryFrame(PTR) { assert(sizeof(*PTR) == sizeof(BlinkVideoFrame)); ZeroMemory(PTR, sizeof(BlinkVideoFrame)); }
#define BlinkZeroMemoryData(PTR) { assert(sizeof(*PTR) == sizeof(BlinkVideoData)); ZeroMemory(PTR, sizeof(BlinkVideoData)); }

#ifndef __cplusplus
#	define BLINK_NULL NULL
#else
#	define BLINK_NULL nullptr
#endif
