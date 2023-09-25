/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <Nplicit.h>
#include <Component.h>

#include "MeshComponent.h"

#define XPX_BUNDLE_MAX (6)

namespace XPX
{
	/* this class combines meshes to make a bundle */
	/* example: Player */

	/// <summary>
	/// A set of multiple animated meshes. Updated according to speed.
	/// </summary>
	class BundleMesh final : public Component
	{
	public:
		BundleMesh() = delete;

	public:
		BundleMesh(const char* character_path, const char* xpx_id);
		~BundleMesh() noexcept;

	public:
		BundleMesh& operator=(const BundleMesh&) = default;
		BundleMesh(const BundleMesh&) = default;

	public:
		typedef IAnimatedMeshSceneNode BundleNode;

	public:
		BundleNode* look_for(const String& name) noexcept
		{
			for (auto& part : mParts)
			{
				if (part->getName() == name)
					return part;
			}

			return nullptr;
		}

	public:
		BundleNode* part_at(const std::size_t& index) const { return mParts[index]; }

		std::size_t count_parts() const { return mParts.size(); }
		const String& xplicit_id() noexcept { return mXplicitId; }

		static bool should_update() { return false; }
		static void update(ClassPtr ptr) {  }

	protected:
		std::vector<BundleNode*> mParts;
		String mXplicitId;
		
	};
}