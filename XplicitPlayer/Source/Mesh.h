/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <Nplicit.h>
#include <Component.h>

#include <MeshComponent.h>

//! generic player bundle offsets.
#define XPLICIT_BUNDLE_HEAD		 (0)
#define XPLICIT_BUNDLE_LEFT_ARM  (1)
#define XPLICIT_BUNDLE_TORSO     (2)
#define XPLICIT_BUNDLE_RIGHT_ARM (3)
#define XPLICIT_BUNDLE_LEFT_LEG  (4)
#define XPLICIT_BUNDLE_RIGHT_LEG (5)
#define XPLICIT_BUNDLE_MAX (6)

namespace Xplicit::Player
{
	class StaticBundleMesh;

	/* this class combines meshes to make a bundle */
	/* example: Epic Bundle */

	class StaticBundleMesh final
	{
	public:
		StaticBundleMesh() = delete;

		explicit StaticBundleMesh(const char* character_path);
		virtual ~StaticBundleMesh() noexcept;

	public:
		StaticBundleMesh& operator=(const StaticBundleMesh&) = default;
		StaticBundleMesh(const StaticBundleMesh&) = default;

	public:
		irr::scene::IMeshSceneNode* node_at(const std::size_t& index) const { return mParts[index].first; }
		irr::scene::IMesh* model_at(const std::size_t& index) const { return mParts[index].second; }

		std::size_t count_parts() const { return mParts.size(); }

	protected:
		std::vector<std::pair<irr::scene::IMeshSceneNode*, irr::scene::IMesh*>> mParts;
		
	};
}