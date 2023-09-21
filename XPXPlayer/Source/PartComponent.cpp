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
		mStud = CAD->getSceneManager()->addMeshSceneNode(CAD->getSceneManager()->getGeometryCreator()->createCubeMesh());

		if (mStud)
		{
			mStud->setName(name);
			mStud->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		}

		this->assign("Destroy", this->part_destroy().c_str());
	}

	bool PartComponent::should_update() noexcept { return false; }

	void PartComponent::update(ClassPtr self) {  }

	String PartComponent::part_destroy() noexcept
	{
		String func_proto = fmt::format("function(self) destroyPart(\"{}\", \"{}\"); end", this->name(), this->parent());
		return func_proto;
	}

	PartComponent::~PartComponent() noexcept
	{
#ifdef __XPLICIT_CLIENT__
		if (mStud)
			mStud->drop();
#endif
	}
}