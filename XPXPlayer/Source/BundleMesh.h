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

	class BundleMesh final : public Component
	{
	public:
		BundleMesh() = delete;

		explicit BundleMesh(const char* character_path, const char* xpx_id);
		~BundleMesh() noexcept;

	public:
		BundleMesh& operator=(const BundleMesh&) = default;
		BundleMesh(const BundleMesh&) = default;

	public:
		irr::scene::IMeshSceneNode* node_at(const std::size_t& index) const { return mParts[index].first; }
		irr::scene::IMesh* model_at(const std::size_t& index) const { return mParts[index].second; }

		std::size_t count_parts() const { return mParts.size(); }
		const String& xplicit_id() noexcept { return mXplicitId; }

		static bool should_update() { return false; }
		static void update(ClassPtr ptr) {  }

	protected:
		std::vector<std::pair<irr::scene::IMeshSceneNode*, irr::scene::IMesh*>> mParts;
		String mXplicitId;
		
	};
}