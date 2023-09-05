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
	StaticMesh::StaticMesh(const char* path, const char* name, const char* group)
		: mPath(path), mNode(nullptr), 
		mPhysics(PHYSICS_NONE), mGroup(group), 
		mName(name), CLuaClass(String("") + group + "." + name),
		mMdl(nullptr)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		String _path = XPLICIT_DIR;
		_path += "/Contents/";
		_path += path;

		mMdl = RENDER->getSceneManager()->getMesh(_path.c_str());

		this->insert("RenderDistance", "1000");
		this->insert("Visible", "true");

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mNode = RENDER->getSceneManager()->addMeshSceneNode(mMdl);
			mPhysics = PHYSICS_COMPLEX;

			mNode->setName(mName.c_str());
			mNode->setPosition(vector3df(0, 0, 0));

			this->insert("Position", "{ X = 0, Y = 0, Z = 0}");
			this->insert("Scale", "{ X = 0, Y = 0, Z = 0}");
			this->insert("Color", "{ A = 255, R = 255, G = 255, B = 255 }");
		}
	}

	StaticMesh::~StaticMesh() noexcept
	{
		if (mNode)
			mNode->drop();

		if (mMdl)
			mMdl->drop();
	}

	void StaticMesh::update(ClassPtr class_ptr)
	{
		auto self = (StaticMesh*)class_ptr;

		if (!self->mNode)
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

		self->mNode->getMaterial(0).DiffuseColor.setRed(self->index_as_number("Color.R"));
		self->mNode->getMaterial(0).DiffuseColor.setGreen(self->index_as_number("Color.G"));
		self->mNode->getMaterial(0).DiffuseColor.setBlue(self->index_as_number("Color.B"));
		self->mNode->getMaterial(0).DiffuseColor.setAlpha(self->index_as_number("Color.A"));
	}

	const String& StaticMesh::path() noexcept { return mPath; }

	bool StaticMesh::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX;  }

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

		for (auto& node : params.WorldNodes)
		{
			for (size_t i = 0; i < 6; i++)
			{
				String part_id(parts[i]);

				if (node.ID == part_id)
				{
					irr::scene::IMeshSceneNode* mesh = static_cast<irr::scene::IMeshSceneNode*>(RENDER->getSceneManager()->getSceneNodeFromName(node.ID.c_str()));

					if (mesh)
						mParts.push_back(std::make_pair(mesh, mesh->getMesh()));
				}
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