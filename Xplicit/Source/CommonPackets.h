/*
 * =====================================================================
 *
 *			XplicitNgin (XplicitRenderer)
 *			Copyright XPX, all rights reserved.
 *
 *			File: Common.h
 *			Purpose: Common packet structures.
 *
 * =====================================================================
 */

#pragma once

#include "NetworkProtocol.h"

namespace Xplicit
{
	// Position packet, to replicate player movement.
	PACKED_STRUCT
	(
		struct XPLICIT_API PositionPacket
		{
			float X; // x-pos
			float Y; // y-pos
			float Z; // z-pos
			float R; // rotation
		};
	)
}