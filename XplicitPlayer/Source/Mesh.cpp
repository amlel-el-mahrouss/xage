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
		: mPath(path), mPhysics(PHYSICS_NONE), mGroup(group), mName(name), CLuaClass(String("World") + "." + group + "." + name)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		String _path = XPLICIT_DIR;
		_path += "/XplicitNgin/Contents/";
		_path += path;

		mMdl = RENDER->getSceneManager()->getMesh(_path.c_str());

		this->insert("RenderDistance", "1000");
		this->insert("Visible", "true");

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mNode = RENDER->getSceneManager()->addMeshSceneNode(mMdl);
			mPhysics = PHYSICS_COMPLEX;

			mNode->setPosition(vector3df(0, 0, 0));

			this->assign("Position.X", "0");
			this->assign("Position.Y", "0");
			this->assign("Position.Z", "0");

			this->assign("Scale.X", std::to_string(mNode->getScale().X).c_str());
			this->assign("Scale.Y", std::to_string(mNode->getScale().Y).c_str());
			this->assign("Scale.Z", std::to_string(mNode->getScale().Z).c_str());

			this->assign("Color.R", std::to_string(mNode->getMaterial(0).AmbientColor.getRed()).c_str());
			this->assign("Color.G", std::to_string(mNode->getMaterial(0).AmbientColor.getGreen()).c_str());
			this->assign("Color.B", std::to_string(mNode->getMaterial(0).AmbientColor.getBlue()).c_str());
			this->assign("Color.A", std::to_string(mNode->getMaterial(0).AmbientColor.getAlpha()).c_str());
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

		auto pos = self->mNode->getPosition();

		pos.X = self->index_as_number("Position.X");
		pos.Y = self->index_as_number("Position.Y");
		pos.Z = self->index_as_number("Position.Z");

		self->mNode->setPosition(pos);

		auto scale = self->mNode->getScale();

		scale.X = self->index_as_number("Scale.X");
		scale.Y = self->index_as_number("Scale.Y");
		scale.Z = self->index_as_number("Scale.Z");

		self->mNode->getMaterial(0).DiffuseColor.setRed(self->index_as_number("Color.R"));
		self->mNode->getMaterial(0).DiffuseColor.setGreen(self->index_as_number("Color.G"));
		self->mNode->getMaterial(0).DiffuseColor.setBlue(self->index_as_number("Color.B"));
		self->mNode->getMaterial(0).DiffuseColor.setAlpha(self->index_as_number("Color.A"));
	}

	const String& StaticMesh::path() noexcept { return mPath; }

	bool StaticMesh::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX;  }

	static auto XPLICIT_BUNDLE_PATH = "Contents/Bundles/";

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
		params.Path += XPLICIT_BUNDLE_PATH;
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

		XPLICIT_INFO("Loaded StaticBundleMesh successfully!");
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