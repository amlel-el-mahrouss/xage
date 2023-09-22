/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "StaticBundleMesh.h"

#include <RoXML.h>
#include <Root.h>

namespace XPX
{
	StaticBundleMesh::StaticBundleMesh(const char* character_path, const char* xplicit_id)
		: mXplicitId(xplicit_id)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		RoXML::RoXMLDocumentParameters params;

		params.Has3D = true; //! We're delaing with 3D Graphics...
		params.NoLua = true; //! No Lua, we don't want RCE...

		//! This is needed, since we aim for convinience.
		//! So we get the XPXNgin/ absolute filesystem path.
		//! We get the Bundles directory.
		//! Load a specific character and show it to screen.
		
		params.Path = XPLICIT_DIR;
		params.Path += "Contents/";
		params.Path += character_path;
		params.WaitFor = true;

		RoXML::RoXMLDocumentParser parser;
		parser.parse(params);

		//! RoXML provides uses cusotmization of these characters.
		//! So that the user can express himself.
		static const const char* XPX_PARTS[XPLICIT_BUNDLE_MAX] = { "Head", "LeftLeg", "RightLeg", "Torso", "LeftArm", "RightArm" };

		for (size_t i = 0; i < XPLICIT_BUNDLE_MAX; ++i)
		{
			auto it = std::find_if(params.WorldNodes.cbegin(), params.WorldNodes.cend(), [&](RoXML::RoXMLNodeDescription desc) -> bool {
				return XPX_PARTS[i] == desc.ID;
			});

			if (it != params.WorldNodes.cend())
			{
				irr::scene::IMeshSceneNode* mesh = static_cast<irr::scene::IMeshSceneNode*>(CAD->getSceneManager()->getSceneNodeFromName(XPX_PARTS[i]));

				if (mesh)
					mParts.push_back(std::make_pair(mesh, mesh->getMesh()));
			}
		}

		XPLICIT_ASSERT(this->count_parts() == XPLICIT_BUNDLE_MAX);

		auto torso = this->node_at(XPLICIT_BUNDLE_TORSO);

		for (auto& part : mParts)
		{
			if (part.first &&
				part.first != torso)
				part.first->setParent(torso);
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