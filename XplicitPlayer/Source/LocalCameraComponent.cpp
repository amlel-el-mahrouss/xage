/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalCameraComponent.h"
#include "Application.h"
#include "MenuUI.h"

namespace Xplicit::Player
{
	LocalCameraComponent::LocalCameraComponent()
		: ClassComponent("World", "Camera"),
		mCamera(nullptr),
		mRotation(0, 0, 0), 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent"))
	{
		mCamera = RENDER->getSceneManager()->addCameraSceneNode();
		XPLICIT_ASSERT(mCamera);

		mCamera->setRotation(vector3df(0, 0, 0));
		mCamera->setName("Camera");

		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("FOV", "90");

		mRotation = mCamera->getRotation();
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept
	{
		if (mCamera)
			mCamera->drop();
	}

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update(void* class_ptr) 
	{
		LocalCameraComponent* cam = (LocalCameraComponent*)class_ptr;

		auto pos = cam->get()->getPosition();

		pos.X = cam->index_as_number("Position.X");
		pos.Y = cam->index_as_number("Position.Y");
		pos.Z = cam->index_as_number("Position.Z");

		cam->get()->setPosition(pos);

		cam->get()->setFOV(cam->index_as_number("FOV"));

		if (cam->mRotation != cam->mCamera->getRotation())
		{
			cam->mRotation = cam->mCamera->getRotation();

			NetworkPacket packet;

			packet.cmd[XPLICIT_NETWORK_CMD_CAM_POS] = NETWORK_CMD_CAM_POS;

			packet.pos[XPLICIT_NETWORK_X] = cam->mRotation.X;
			packet.pos[XPLICIT_NETWORK_Y] = cam->mRotation.Y;
			packet.pos[XPLICIT_NETWORK_Z] = cam->mRotation.Z;

			packet.channel = XPLICIT_CHANNEL_DATA;

			cam->mNetwork->send(packet);
		}
	}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}