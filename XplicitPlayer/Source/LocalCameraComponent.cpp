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
		: Component(), 
		mCamera(nullptr),
		mRotation(0, 0, 0), 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent"))
	{
		mCamera = RENDER->getSceneManager()->addCameraSceneNodeFPS();
		XPLICIT_ASSERT(mCamera);

		mCamera->setRotation(vector3df(0, 0, 0));
		mCamera->setName("Camera");

		mRotation = mCamera->getRotation();
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			mCamera->drop();

		if (mRotation != mCamera->getRotation())
		{
			mRotation = mCamera->getRotation();

			NetworkPacket packet;

			packet.cmd[XPLICIT_NETWORK_CMD_CAM_POS] = NETWORK_CMD_CAM_POS;

			packet.pos[XPLICIT_NETWORK_X] = mRotation.X;
			packet.pos[XPLICIT_NETWORK_Y] = mRotation.Y;
			packet.pos[XPLICIT_NETWORK_Z] = mRotation.Z;

			packet.channel = XPLICIT_CHANNEL_DATA;

			mNetwork->send(packet);
		}
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update(void* class_ptr) 
	{
		LocalCameraComponent* cam = (LocalCameraComponent*)class_ptr;
		(void)cam;
	}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}