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
		static const const char* parts[6] = { "Head", "LeftLeg", "RightLeg", "Torso", "LeftArm", "RightArm" };

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