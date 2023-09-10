/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "MeshComponent.h"
#include "GameVar.h"

namespace XPX
{
	MeshComponent::MeshComponent(const char* path, const char* name, const char* group)
		:
		mPath(path), mNode(nullptr),
		mPhysics(PHYSICS_NONE), mGroup(group),
		mName(name), CLuaClass(String(group) + "." + name),
		mMdl(nullptr)
	{
		XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		String _path = XPLICIT_DIR;
		_path += path;

		std::cout << _path << std::endl;

		mMdl = RENDER->getSceneManager()->getMesh(_path.c_str());

		this->insert("Visible", "true");

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mNode = RENDER->getSceneManager()->addAnimatedMeshSceneNode(mMdl);
			mPhysics = PHYSICS_COMPLEX;

			mNode->setName(mName.c_str());
			mNode->setPosition(vector3df(0, 0, 0));

			this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
			this->insert("Scale", "{ X = 0, Y = 0, Z = 0}");
			this->insert("Color", "{ A = 255, R = 255, G = 255, B = 255 }");

			mMdl->setMaterialFlag(video::EMF_LIGHTING, false);
		}
	}

	MeshComponent::~MeshComponent() noexcept
	{
		if (mNode)
			mNode->drop();

		if (mMdl)
			mMdl->drop();
	}

	void MeshComponent::update(ClassPtr class_ptr)
	{
		auto self = (MeshComponent*)class_ptr;

		if (!self->mNode)
			return;

		self->mNode->setVisible(self->index_as_bool("Visible"));

		if (!self->mNode->isVisible() ||
			self->mNode->getMaterial(0).AmbientColor.getAlpha() == 0 ||
			self->mNode->getMaterial(0).DiffuseColor.getAlpha() == 0)
			return;

		auto pos = self->mNode->getPosition();

		pos.X = self->index_as_number("Position.X");
		pos.Y = self->index_as_number("Position.Y");
		pos.Z = self->index_as_number("Position.Z");

		self->mNode->setPosition(pos);

		auto scale = self->mNode->getScale();

		scale.X = self->index_as_number("Scale.X");
		scale.Y = self->index_as_number("Scale.Y");
		scale.Z = self->index_as_number("Scale.Z");

		self->mNode->setScale(scale);

		self->mNode->getMaterial(0).AmbientColor.setRed(self->index_as_number("Color.R"));
		self->mNode->getMaterial(0).AmbientColor.setGreen(self->index_as_number("Color.G"));
		self->mNode->getMaterial(0).AmbientColor.setBlue(self->index_as_number("Color.B"));
		self->mNode->getMaterial(0).AmbientColor.setAlpha(self->index_as_number("Color.A"));

		self->mNode->getMaterial(0).DiffuseColor.setRed(self->index_as_number("Color.R"));
		self->mNode->getMaterial(0).DiffuseColor.setGreen(self->index_as_number("Color.G"));
		self->mNode->getMaterial(0).DiffuseColor.setBlue(self->index_as_number("Color.B"));
		self->mNode->getMaterial(0).DiffuseColor.setAlpha(self->index_as_number("Color.A"));
	}

	const String& MeshComponent::path() noexcept { return mPath; }

	bool MeshComponent::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX; }
}