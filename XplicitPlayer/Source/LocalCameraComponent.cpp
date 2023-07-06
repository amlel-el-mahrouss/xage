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

#include <Overlay/OgreOverlayContainer.h>
#include <Overlay/OgreOverlay.h>
#include <Overlay/OgreOverlayManager.h>

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
	
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		// Create an overlay
		Ogre::Overlay* overlay = overlayManager.create("OverlayName");

		// Create a panel
		Ogre::OverlayContainer* panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "PanelName"));
		panel->setPosition(0.0, 0.0);
		panel->setDimensions(0.1, 0.1);
		panel->setMaterialName("BaseWhite");
		// Add the panel to the overlay
		overlay->add2D(panel);

		// Show the overlay
		overlay->show();
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