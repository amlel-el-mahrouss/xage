/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <NginCore.h>
#include <CLua.hpp>
#include <NplicitNgine.h>
#include <Component.h>
#include <Root.h>

namespace XPX
{
	/* this classes gives an component a model. */
	class MeshComponent final
	{
	public:
		MeshComponent(const char* path, const std::size_t id);
		~MeshComponent() noexcept;
		MeshComponent() = delete;

	public:
		XPLICIT_COPY_DEFAULT(MeshComponent);

	public:
		const char* name() noexcept { return "MeshComponent"; }

		irr::scene::ISceneNode* node() const { return mNode; }
		irr::scene::IMesh* model() const { return mMdl; }

	public:
		const String& path() noexcept;
		bool has_physics() noexcept;

	public:
		static bool should_update();
		static void update(ClassPtr class_ptr);

	protected:
		irr::scene::IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		irr::scene::IAnimatedMesh* mMdl; // Model Data pointer, generic
		PHYSICS_TYPE mPhysics; // What kind of physics we have here?
		String mPath;

	};

}