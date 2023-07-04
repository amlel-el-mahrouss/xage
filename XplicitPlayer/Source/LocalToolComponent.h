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

namespace Xplicit::Player
{
	class LocalToolComponent final : public Component
	{
	public:
		LocalToolComponent() = delete;
		~LocalToolComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(LocalToolComponent);

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;
	
	public:
		bool should_update() noexcept override;
		void update() noexcept override;

	public:
		explicit LocalToolComponent(const char* name, 
			const char* mesh = nullptr,
			const char* parent = "Game") noexcept;

	public:
		StaticMesh* get_mesh() const noexcept;
		XAttribute& get_attribute() noexcept;

	private:
		std::unique_ptr<StaticMesh> mMeshPtr;
		XAttribute mAttribute;

	private:
		String mParent;
		String mName;

	};
}