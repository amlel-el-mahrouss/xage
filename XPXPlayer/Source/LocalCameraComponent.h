/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Component.h>
#include <ClassComponent.h>
#include <NetworkComponent.h>
#include <irrlicht.h>

namespace XPX
{
	// The player view in-person.
	class LocalCameraComponent final : public ClassComponent
	{
	public:
		LocalCameraComponent() noexcept;
		~LocalCameraComponent() noexcept override;
		
		LocalCameraComponent& operator=(const LocalCameraComponent&) = default;
		LocalCameraComponent(const LocalCameraComponent&) = default;

	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() { return false; }
		static void update(void* class_ptr);

	public:
		irr::scene::ICameraSceneNode* get() noexcept;

	private:
		irr::scene::ICameraSceneNode* mCamera;
		NetworkComponent* mNetwork;
		irr::core::vector3df mLookAt;

	};
}
