/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#include <CLua/CLua.hpp>

#include "Component.h"
#include "Root.h"

namespace Xplicit
{
	/* this classes gives an component a model. */
	class XPLICIT_API MeshComponent : public Lua::CLuaClass
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
		irr::scene::IMesh* model() const { return mMdl; }

	public:
		const String& path() noexcept;
		bool has_physics() noexcept;

	public:
		static bool should_update() { return true; }
		static void update(ClassPtr class_ptr);

	protected:
		irr::scene::IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		irr::scene::IMesh* mMdl; // Model Data pointer, generic

	private:
		PHYSICS_TYPE mPhysics; // What kind of physics we have here?

	private:
		String mGroup;
		String mName;
		String mPath; // Filesystem path (must be)

	};

}