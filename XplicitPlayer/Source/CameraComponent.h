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
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

	public:
		CameraComponent& operator=(const CameraComponent&) = default;
		CameraComponent(const CameraComponent&) = default;

	public:
		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;

	public:
		ICameraSceneNode* get() noexcept;

	private:
		ICameraSceneNode* m_camera;
		ITexture* m_cursor;

	};
}
