/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <ClassComponent.h>
#include <NetworkProtocol.h>
#include <Nplicit.h>

namespace XPX
{
	typedef irr::scene::ISceneNode* CorePartPtr;

	class PartComponent final : public ClassComponent
	{
	public:
		PartComponent(const char* name, const char* parent,
			Vector<NetworkFloat> pos = Vector<NetworkFloat>(0, 0, 0),
			Vector<NetworkFloat> scale = Vector<NetworkFloat>(0, 0, 0),
			Color<NetworkFloat> clr = Color<NetworkFloat>(0, 0, 0));

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