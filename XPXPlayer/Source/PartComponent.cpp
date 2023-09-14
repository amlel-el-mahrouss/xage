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
		Vector<float> pos,
		Vector<float> scale,
		Color<float> clr)
		:
		ClassComponent(pos, scale, clr, nullptr, parent, name),
		mStud(nullptr)
	{
		mStud = CAD->getSceneManager()->addMeshSceneNode(CAD->getSceneManager()->getGeometryCreator()->createCubeMesh());

		if (mStud)
		{
			mStud->setName(name);
			mStud->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		}

		this->insert("Destroy", this->part_destroy().c_str());
		this->insert("__gc", this->part_destroy().c_str());
	}

	void PartComponent::update(ClassPtr cls)
	{
		ClassComponent::update(cls);

		PartComponent* self = (PartComponent*)cls;

		if (CAD)
		{
			if (self->index_as_bool("Locked"))
				return;

			if (self->mStud)
			{
				self->mStud->setScale(vector3df(self->scale().X, self->scale().Y, self->scale().Z));
				self->mStud->setPosition(vector3df(self->pos().X, self->pos().Y, self->pos().Z));

				self->mStud->setRotation(vector3df(self->index_as_number("Rotation.X"), self->index_as_number("Rotation.Y"), self->index_as_number("Rotation.Z")));

				self->mStud->getMaterial(0).DiffuseColor.set(self->color().A, self->color().R, self->color().G, self->color().B);
				self->mStud->getMaterial(0).AmbientColor.set(self->color().A, self->color().R, self->color().G, self->color().B);
			}
		}
	}

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