/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <RoXML.h>
#include <Root.h>

#include "Mesh.h"

namespace Xplicit::Player
{
	MeshComponent::MeshComponent(const char* path, const char* name, const char* group)
		: 
		mPath(path), mNode(nullptr), 
		mPhysics(PHYSICS_NONE), mGroup(group), 
		mName(name), CLuaClass(String("") + group + "." + name),
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
			mNode = RENDER->getSceneManager()->addOctreeSceneNode(mMdl);
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

		auto pos = self->mNode->getPosition();

		self->mNode->setVisible(self->index_as_bool("Visible"));

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

	bool MeshComponent::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX;  }

	StaticBundleMesh::StaticBundleMesh(const char* character_path)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		RoXML::RoXMLDocumentParameters params;

		params.Has3D = true; //! We're delaing with 3D Graphics...
		params.NoLua = true; //! No Lua, we don't want RCE...

		//! This is needed, since we aim for convinience.
		//! So we get the XplicitNgin/ absolute filesystem path.
		//! We get the Bundles directory.
		//! Load a specific character and show it to screen.
		
		params.Path = XPLICIT_DIR;
		params.Path += "/";
		params.Path += character_path;
		params.WaitFor = true;

		RoXML::RoXMLDocumentParser parser;
		parser.parse(params);

		//! RoXML provides uses cusotmization of these characters.
		//! So that the user can express himself.
		const char* parts[6] = { "Head", "LeftLeg", "RightLeg", "Torso", "LeftArm", "RightArm" };

		for (size_t i = 0; i < XPLICIT_BUNDLE_MAX; ++i)
		{
			auto it = std::find_if(params.WorldNodes.cbegin(), params.WorldNodes.cend(), [&](RoXML::RoXMLNodeDescription desc) -> bool {
				return parts[i] == desc.ID;
				});

			if (it != params.WorldNodes.cend())
			{
				irr::scene::IMeshSceneNode* mesh = static_cast<irr::scene::IMeshSceneNode*>(RENDER->getSceneManager()->getSceneNodeFromName(parts[i]));

				if (mesh)
					mParts.push_back(std::make_pair(mesh, mesh->getMesh()));
			}
		}
	}

	StaticBundleMesh::~StaticBundleMesh() noexcept
	{
		for (auto& part : mParts)
		{
			if (part.first)
				part.first->drop();

			if (part.second)
				part.second->drop();
		}
	}
}