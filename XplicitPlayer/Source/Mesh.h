/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <Nplicit.h>
#include <Component.h>

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
	class MeshComponent;
	class StaticBundleMesh;

	/* this classes gives an component a model. */
	class MeshComponent : public Lua::CLuaClass
	{
	public:
		MeshComponent() = delete;

	public:
		explicit MeshComponent(const char* path, const char* name, const char* group);
		virtual ~MeshComponent() noexcept;

	public:
		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;
		
	public:
		const char* name() noexcept { return "MeshComponent"; }

		irr::scene::ISceneNode* node() const { return mNode; }
		irr::scene::IMesh* operator->() const { return mMdl; }

	public:
		const String& path() noexcept;
		bool has_physics() noexcept;

	public:
		static bool should_update() { return true; }
		static void update(ClassPtr class_ptr);

	protected:
		irr::scene::ISceneNode* mNode; // Model Data pointer, generic
		irr::scene::IMesh* mMdl; // Model Data pointer, generic

	private:
		PHYSICS_TYPE mPhysics; // What kind of physics we have here?

	private:
		String mGroup;
		String mName;
		String mPath; // Filesystem path (must be)

	};

	/* this class combines meshes to make a bundle */
	/* example: Epic Bundle */

	class StaticBundleMesh
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