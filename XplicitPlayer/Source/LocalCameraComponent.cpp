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
	LocalCameraComponent::LocalCameraComponent(const char* name)
		: Component(), mCamera(nullptr)
	{
		mCamera = RENDER->getSceneManager()->addCameraSceneNode();
		mCamera->setName("Camera");

		XPLICIT_ASSERT(mCamera);
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			mCamera->drop();
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update() {}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}