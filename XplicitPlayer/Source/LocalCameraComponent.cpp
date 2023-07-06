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
		auto ptr = Root::get_singleton_ptr();

		mCamera = ptr->Ogre3D_Scene->createCamera(name);
		XPLICIT_ASSERT(mCamera);

		mCamera->setNearClipDistance(5); // specific to this sample
		mCamera->setAutoAspectRatio(true);
		mCamera->setDebugColour(Ogre::ColourValue(255, 40, 40, 40));

		Root::get_singleton_ptr()->getRenderWindow()->addViewport(mCamera);
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			delete mCamera;
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update() {}

	Ogre::Camera* LocalCameraComponent::get() noexcept { return mCamera; }
}