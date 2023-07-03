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

namespace Xplicit::Player
{
	// The player view in-person.
	class LocalCameraComponent final : public Component
	{
	public:
		LocalCameraComponent();
		~LocalCameraComponent() noexcept override;
		
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
