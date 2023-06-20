/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalCameraComponent.h"
#include "Application.h"
#include "GameMenuUI.h"

namespace Xplicit::Player
{
	LocalCameraComponent::LocalCameraComponent()
		: Component(), mCamera(nullptr)
	{
		mCamera = IRR->getSceneManager()->addCameraSceneNodeFPS();

		XPLICIT_ASSERT(mCamera);

		mCamera->setName(this->name());

		IRR->getCursorControl()->setVisible(false);

		mCursor = IRR->getVideoDriver()->getTexture("cursor.png");

		XPLICIT_ASSERT(mCursor);
	}

	LocalCameraComponent::~LocalCameraComponent()
	{
		if (mCamera)
			mCamera->drop();

	}

	LocalCameraComponent::COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("CameraComponent"); }

	void LocalCameraComponent::update()
	{
		if (!mCursor)
			return;

		IRR->getVideoDriver()->draw2DImage(mCursor,
			IRR->getCursorControl()->getPosition(),
			rect<s32>(0, 0, 38, 38),
			nullptr,
			SColor(255, 255, 255, 255),
			true);
	}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}