/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Mesh.h"

#include <Util.h>
#include <Component.h>
#include <CommonEngine.h>
#include <ClassComponent.h>

namespace Xplicit::Player
{
	class GearComponent final : public ClassComponent
	{
	public:
		GearComponent() = delete;
		~GearComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(GearComponent);

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;
	
	public:
		bool should_update() noexcept override;
		void update() noexcept override;

	public:
		explicit GearComponent(const char* name, 
			const char* mesh = nullptr,
			const char* parent = "Game") noexcept;

	public:
		StaticMesh* get_mesh() const noexcept;

	private:
		std::unique_ptr<StaticMesh> mMeshPtr;

	private:
		String mParent;
		String mName;

	};
}