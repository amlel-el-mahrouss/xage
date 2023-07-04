/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
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
		mCamera = RENDER->_getCurrentSceneManager()->createCamera("Camera");
		XPLICIT_ASSERT(mCamera);

		mCursor = RENDER->getTextureManager()->load("cursor.png", XPLICIT_RES_GROUP, Ogre::TEX_TYPE_2D, -1, 1.0F, Ogre::PF_UNKNOWN, true);
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			delete mCamera;
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update()
	{
		
	}

	Ogre::Camera* LocalCameraComponent::get() noexcept { return mCamera; }
}