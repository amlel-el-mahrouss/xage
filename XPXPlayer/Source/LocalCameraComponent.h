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

#include <Root.h>

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

		static bool should_update() { return true; }
		static void update(void* class_ptr);

	public:
		ICameraSceneNode* get() noexcept;

	private:
		ICameraSceneNode* mCamera;
		vector3df mLookAt;
		NetworkComponent* mNetwork;

	};
}
