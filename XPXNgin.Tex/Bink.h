/*
 * =====================================================================
 *
 *			XAGE.Tex
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: XVID Bink codec.
 *
 * =====================================================================
 */

#pragma once

#include "XIFF.h"

//! XAGE Bink is based on XIFF. A Format based on IFF.
//! IFF was made by EA back for the amiga.
//! We use this for video stuff.

typedef struct XiffVideoHeader BinkHeader;

#define BINK_VIDEO_FORMAT ".vid"

#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

//! to send to directx.
typedef char BinkData[4096];

#define XAGE_BINK_MALLOC malloc
#define XAGE_BINK_FREE free

static inline BinkHeader* BinkReadContainer(FILE* file)
{
	BinkHeader* readHdr = XAGE_BINK_MALLOC(sizeof(BinkHeader));

	if (fread(readHdr, sizeof(BinkHeader), SEEK_SET, file) == sizeof(BinkHeader))
		return readHdr;

	XAGE_BINK_FREE(readHdr);
	return NULL;
}

#define XIFF_FORMAT_BINK (0x5775FF)

static inline BOOL BinkContainerCheck(BinkHeader* hdr)
{
	return strcmp(hdr->mag, XIFF_VIDEO_MAG) &&
		hdr->format_type = XIFF_FORMAT_BINK;
}

struct BinkPrivateData
{
	SIZE_T f_szData;
	WORD f_iFormat;
	WORD f_iType;
	BinkData f_pData;
};

enum
{
	BINK_DATA_TYPE_VIDEO,
	BINK_DATA_TYPE_AUDIO,
};

enum
{
	BINK_DATA_FORMAT_R32G32B32A32,
	BINK_DATA_FORMAT_R16G16B16A16,
};

enum
{
	BINK_DATA_VIEWPORT_1280x720,
	BINK_DATA_VIEWPORT_800x600,
};

struct BinkVideoFrame
{
	UINT f_iTexture;

	SIZE_T f_szOffsets;
	INT_PTR f_szOffset[64];

	struct BinkPrivateData f_pPrivateData;

	struct BinkVideoFrame* f_pNext;
};

#include <assert.h>

#define BinkZeroMemoryFrame(PTR) { assert(sizeof(*PTR) == sizeof(BinkVideoFrame)); ZeroMemory(PTR, sizeof(BinkVideoFrame)); }
#define BinkZeroMemoryData(PTR) { assert(sizeof(*PTR) == sizeof(BinkVideoData)); ZeroMemory(PTR, sizeof(BinkVideoData)); }

#ifndef __cplusplus
#	define BINK_NULL NULL
#else
#	define BINK_NULL nullptr
#endif

static inline BinkVideoFrame* BinkReadContainerFile(FILE* fp)
{
	BinkVideoFrame* pBase = BINK_NULL;
	pBase = XAGE_BINK_MALLOC(sizeof(BinkVideoFrame));

	pBase->f_szOffset = 0UL;

	BinkPrivateData tmp;
	BinkZeroMemoryData(&tmp);

	BinkVideoFrame* pIt = pBase;

	while (fread(&tmp, sizeof(BinkVideoFrame), SEEK_CUR, fp) == sizeof(BinkVideoFrame))
	{
		if (tmp.f_szData < 1)
			continue;

		pIt->f_pPrivateData = tmp;
		pIt->f_szOffsets[pIt->f_szOffset] = (INT_PTR)pIt;

		++pIt->f_szOffset;

		if (pIt->f_szOffset > 64)
		{
			BinkVideoFrame* pNextIt = BINK_NULL;
			pNextIt = XAGE_BINK_MALLOC(sizeof(BinkVideoFrame));

			pNextIt->f_szOffset = 0UL;

			pIt->f_pNext = pNextIt;
		}
	}

	return pBase;
}