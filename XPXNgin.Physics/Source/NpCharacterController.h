/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: PhysX based character controller.
 *
 * =====================================================================
 */

#pragma once

#include "NplicitNgine.h"

#include <AINgine.h>
#include <Root.h>

namespace XPX
{
	enum
	{
		NP_ANIM_START, //! start animation generation.
		NP_ANIM_END, //! end animation generation.
		NP_ANIM_PROCESSING, //! error code, tell that we're busy.
	};
}