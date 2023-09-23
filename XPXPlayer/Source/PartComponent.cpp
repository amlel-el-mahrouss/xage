/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "PartComponent.h"

namespace XPX
{
	PartComponent::PartComponent(const char* name, const char* parent,
		Vector<NetworkFloat> pos,
		Vector<NetworkFloat> scale,
		Color<NetworkFloat> clr)
		:
		ClassComponent(pos, scale, clr, nullptr, parent, name),
		mStud(nullptr)
	{
		mStud = CAD->getSceneManager()->addCubeSceneNode();

		if (mStud)
		{
			mStud->setName(name);
			mStud->setMaterialFlag(EMF_LIGHTING, true);
		}
	}

	bool PartComponent::should_update() noexcept { return false; }

	void PartComponent::update(ClassPtr self) {  }

	PartComponent::~PartComponent() noexcept
	{
#ifdef __XPLICIT_CLIENT__
		if (mStud)
			mStud->drop();
#endif
	}
}