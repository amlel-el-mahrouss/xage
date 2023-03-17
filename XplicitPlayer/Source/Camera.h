/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Camera.h
 *			Purpose: Client-side misc instances
 *
 * =====================================================================
 */

#pragma once

#include "Component.h"
#include "ApplicationContext.h"

namespace Xplicit::Client
{
	// The 'player' in-person.
	class CameraComponent : public Component
	{
	public:
		CameraComponent();
		virtual ~CameraComponent();

		CameraComponent& operator=(const CameraComponent&) = default;
		CameraComponent(const CameraComponent&) = default;

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
