/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Camera.cpp
 *			Purpose: Client-side camera
 *
 * =====================================================================
 */

#include "Application.h"
#include "Camera.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	CameraInstance::CameraInstance()
		: Instance(), m_camera(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeMaya(nullptr);
		XPLICIT_ASSERT(m_camera);

		m_camera->setName(this->name());

		IRR->getCursorControl()->setVisible(false);

		m_cursor = IRR->getVideoDriver()->getTexture("cursor.png");

		XPLICIT_ASSERT(m_cursor);
	}

	CameraInstance::~CameraInstance()
	{
		if (m_camera)
			m_camera->drop();

	}

	CameraInstance::INSTANCE_TYPE CameraInstance::type() noexcept { return INSTANCE_CAMERA; }
	const char* CameraInstance::name() noexcept { return ("CameraInstance"); }

	void CameraInstance::update()
	{
		if (!m_cursor)
			return;

		IRR->getVideoDriver()->draw2DImage(m_cursor, IRR->getCursorControl()->getPosition(), rect<s32>(0, 0, 38, 38), nullptr, SColor(255, 255, 255, 255), true);
	}

	irr::scene::ICameraSceneNode* CameraInstance::get() noexcept { return m_camera; }
}