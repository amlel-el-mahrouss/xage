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
		: mPath(path), mPhysics(PHYSICS_NONE), mGroup(group), mName(name), CLuaClass(String("World") + "." + name)
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
		}
	}

	StaticMesh::~StaticMesh() noexcept
	{
		if (mNode)
			mNode->drop();

		if (mMdl)
			mMdl->drop();
	}

	const String& StaticMesh::path() noexcept
	{
		return mPath;
	}

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
		parser.load(params);

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