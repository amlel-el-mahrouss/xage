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

#include "Instance.h"
#include "ApplicationContext.h"

namespace Xplicit::Client
{
	// The 'player' in-person.
	class CameraInstance : public Instance
	{
	public:
		CameraInstance();
		virtual ~CameraInstance();

		CameraInstance& operator=(const CameraInstance&) = default;
		CameraInstance(const CameraInstance&) = default;

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
