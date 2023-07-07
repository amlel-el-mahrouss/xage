/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Component.h>
#include <cad-engine/irrlicht.h>

namespace Xplicit::Player
{
	// The player view in-person.
	class LocalCameraComponent final : public Component
	{
	public:
		LocalCameraComponent(const char* name);
		~LocalCameraComponent() noexcept override;
		
		LocalCameraComponent& operator=(const LocalCameraComponent&) = default;
		LocalCameraComponent(const LocalCameraComponent&) = default;

	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		void update() override;

	public:
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* mCamera;

	};
}
