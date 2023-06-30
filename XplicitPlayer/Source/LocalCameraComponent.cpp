/*
 * =====================================================================
 *
 *			XplicitPlayer
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
		mCamera = RENDER->getSceneManager()->addCameraSceneNode();
		XPLICIT_ASSERT(mCamera);

		mCamera->setName(this->name());

		RENDER->getCursorControl()->setVisible(false);

		mCursor = RENDER->getVideoDriver()->getTexture("cursor.png");
		XPLICIT_ASSERT(mCursor);
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			mCamera->drop();

		if (mCursor)
			mCursor->drop();
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update()
	{
		if (!mCursor)
			return;

		RENDER->getVideoDriver()->draw2DImage(mCursor,
			RENDER->getCursorControl()->getPosition(),
			rect(0, 0, 38, 38),
			nullptr,
			SColor(255, 255, 255, 255),
			true);

		XPLICIT_INFO("LocalCameraComponent:Update");

	}

	ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}