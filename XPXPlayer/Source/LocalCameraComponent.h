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
#include <NetworkComponent.h>

#include <Root.h>

namespace XPX
{
	// The player view in-person.
	class LocalCameraComponent final : public Component
	{
	public:
		LocalCameraComponent() noexcept;
		~LocalCameraComponent() noexcept override;
		
	public:
		LocalCameraComponent& operator=(const LocalCameraComponent&) = default;
		LocalCameraComponent(const LocalCameraComponent&) = default;

	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() { return true; }
		static void update(void* class_ptr);

	public:
		Renderer::DriverCameraSystem* get() noexcept;

	private:
		Renderer::DriverCameraSystem* mCamera;
		Vector<float> mLookAt;
		NetworkComponent* mNetwork;

	};
}
