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

#include <RXML.h>
#include <Root.h>

namespace XPX
{
	BundleMesh::BundleMesh(const char* rxml_path, const std::vector<String>& filter)
	{
		static XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		RXML::RXMLDocumentParams params;

		params.Has3D = true; //! We're delaying with 3D Graphics...
		params.NoLua = true; //! No Lua, we don't want RCE...

		//! This is needed, since we aim for convenience.
		//! So we get the XPXNgin/ absolute filesystem path.
		//! We get the Bundles directory.
		//! Load a specific character and show it to screen.
		
		params.Path = XPLICIT_DIR;
		params.Path += "Contents/";
		params.Path += rxml_path;
		params.WaitFor = true;

		RXML::RXMLDocument parser;
		parser.parse(params);

		auto filter_cnt = filter.size();

		for (size_t i = 0; i < filter_cnt; ++i)
		{
			auto it = std::find_if(params.WorldNodes.cbegin(), params.WorldNodes.cend(),
				[&](RXML::RoXMLNodeDescription desc) -> bool {
					return filter[i] == desc.ID;
				});

			if (it != params.WorldNodes.cend())
			{
				BundleNode* mesh = static_cast<BundleNode*>(CAD->getSceneManager()->getSceneNodeFromName(filter[i].c_str()));

				if (mesh)
					mParts.push_back(mesh);
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