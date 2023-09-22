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
		mStud = CAD->getSceneManager()->addCubeSceneNode(10.F, 
			CAD->getSceneManager()->getActiveCamera(),
			-1, 
			vector3df(0, 0, 0), 
			vector3df(0, 0, 0),
			vector3df(5, 2, 1));

		if (mStud)
		{
			mStud->setName(name);
			mStud->setMaterialFlag(EMF_LIGHTING, false);
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