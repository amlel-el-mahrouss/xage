/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ClassComponent.h"

namespace Xplicit
{
	class XPLICIT_API PartComponent final : public ClassComponent
	{
	public:
		PartComponent(const char* name, const char* parent, 
			Vector<float> pos = Vector<float>(0,0,0), 
			Vector<float> scale = Vector<float>(0, 0, 0),
			Color<float> clr = Color<float>(0, 0, 0))
			:
			ClassComponent(pos, scale, clr, nullptr, parent, name)
			,mStud(nullptr)
		{
#ifdef __XPLICIT_CLIENT__
			mStud = RENDER->getSceneManager()->addMeshSceneNode(RENDER->getSceneManager()->getGeometryCreator()->createCubeMesh());

			if (mStud)
			{
				mStud->setName(name);
				mStud->setMaterialFlag(irr::video::EMF_LIGHTING, false);
			}
#endif

			this->assign("Destroy", this->part_service_destroy().c_str());
		}

		~PartComponent()
		{
#ifdef __XPLICIT_CLIENT__
			if (mStud)
				mStud->drop();
#endif
		}

	private:
		String part_service_destroy() noexcept
		{
			String func_proto = std::format("function(self) World.PartService.Destroy(\"{}\"); end",
				this->name());

			return func_proto;
		}

	public:
		static void update(ClassPtr cls)
		{
			ClassComponent::update(cls);

#ifdef __XPLICIT_CLIENT__
			PartComponent* self = (PartComponent*)cls;

			if (RENDER)
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
#endif
		}

	public:
		XPLICIT_COPY_DEFAULT(PartComponent);

	private:
		irr::scene::ISceneNode* mStud;

	};
}