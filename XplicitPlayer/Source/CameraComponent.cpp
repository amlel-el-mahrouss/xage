/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Camera.cpp
 *			Purpose: Client-side Camera
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "CameraComponent.h"
#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::Player
{
	CameraComponent::CameraComponent()
		: Component(), m_camera(nullptr)
	{
		m_camera = IRR->getSceneManager()->addCameraSceneNodeFPS();

		XPLICIT_ASSERT(m_camera);

		m_camera->setName(this->name());

		IRR->getCursorControl()->setVisible(false);

		m_cursor = IRR->getVideoDriver()->getTexture("cursor.png");

		XPLICIT_ASSERT(m_cursor);
	}

	CameraComponent::~CameraComponent()
	{
		if (m_camera)
			m_camera->drop();

	}

	CameraComponent::INSTANCE_TYPE CameraComponent::type() noexcept { return INSTANCE_CAMERA; }

	const char* CameraComponent::name() noexcept { return ("CameraComponent"); }

	void CameraComponent::update()
	{
		if (!m_cursor)
			return;

		IRR->getVideoDriver()->draw2DImage(m_cursor,
			IRR->getCursorControl()->getPosition(),
			rect<s32>(0, 0, 38, 38),
			nullptr,
			SColor(255, 255, 255, 255),
			true);
	}

	irr::scene::ICameraSceneNode* CameraComponent::get() noexcept { return m_camera; }
}