/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Camera.h
 *			Purpose: Client-side Camera
 *
 * =====================================================================
 */

#pragma once

#include <Component.h>

namespace Xplicit::Player
{
	// The player view in-person.
	class LocalCameraComponent final : public Component
	{
	public:
		LocalCameraComponent();
		~LocalCameraComponent() override;
		
		LocalCameraComponent& operator=(const LocalCameraComponent&) = default;
		LocalCameraComponent(const LocalCameraComponent&) = default;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		void update() override;
		 
		ICameraSceneNode* get() noexcept;

	private:
		ICameraSceneNode* mCamera;
		ITexture* mCursor;

	};
}
