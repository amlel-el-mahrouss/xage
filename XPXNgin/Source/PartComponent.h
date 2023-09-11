/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ClassComponent.h"
#include "GameVar.h"

#include <Nplicit.h>

namespace XPX
{
	typedef irr::scene::ISceneNode* CorePartPtr;

	class XPLICIT_API PartComponent final : public ClassComponent
	{
	public:
		PartComponent(const char* name, const char* parent,
			Vector<float> pos = Vector<float>(0, 0, 0),
			Vector<float> scale = Vector<float>(0, 0, 0),
			Color<float> clr = Color<float>(0, 0, 0));

		~PartComponent() noexcept;

	private:
		String part_destroy() noexcept;

	public:
		static void update(ClassPtr cls);

	public:
		XPLICIT_COPY_DEFAULT(PartComponent);

	private:
		CorePartPtr mStud;

	};
}