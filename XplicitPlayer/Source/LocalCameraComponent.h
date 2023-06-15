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
	class LocalCameraComponent : public Component
	{
	public:
		LocalCameraComponent();
		virtual ~LocalCameraComponent();

	public:
		LocalCameraComponent& operator=(const LocalCameraComponent&) = default;
		LocalCameraComponent(const LocalCameraComponent&) = default;

	public:
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;

	public:
		ICameraSceneNode* get() noexcept;

	private:
		ICameraSceneNode* mCamera;
		ITexture* mCursor;

	};
}
