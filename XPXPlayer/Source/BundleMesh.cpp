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

#include "BundleMesh.h"

#include <RoXML.h>
#include <Root.h>

namespace XPX
{
	BundleMesh::BundleMesh(const char* character_path, const char* xplicit_id)
		: mXplicitId(xplicit_id)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		RoXML::RoXMLDocumentParameters params;

		params.Has3D = true; //! We're delaing with 3D Graphics...
		params.NoLua = true; //! No Lua, we don't want RCE...

		//! This is needed, since we aim for convinence.
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
		static const const char* XPX_PARTS[XPX_BUNDLE_MAX] = { "Head", "LeftLeg", "RightLeg", "Torso", "LeftArm", "RightArm" };

		for (size_t i = 0; i < XPX_BUNDLE_MAX; ++i)
		{
			auto it = std::find_if(params.WorldNodes.cbegin(), params.WorldNodes.cend(), 
				[&](RoXML::RoXMLNodeDescription desc) -> bool {
				return XPX_PARTS[i] == desc.ID;
			});

			if (it != params.WorldNodes.cend())
			{
				BundleNode* mesh = static_cast<BundleNode*>(CAD->getSceneManager()->getSceneNodeFromName(XPX_PARTS[i]));

				if (mesh)
				{
					mesh->setName(fmt::format("{}{}{}", mXplicitId, "_", XPX_PARTS[i]).c_str());
					mParts.push_back(mesh);
				}
			}
		}
	}

	BundleMesh::~BundleMesh() noexcept
	{
		for (auto& part : mParts)
		{
			if (part)
				part->drop();
		}
	}
}