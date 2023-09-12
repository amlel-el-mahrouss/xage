/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalCameraComponent.h"
#include "Application.h"
#include "MenuUI.h"

namespace XPX
{
	LocalCameraComponent::LocalCameraComponent() noexcept
		: ClassComponent("World", "Camera"),
		mCamera(nullptr),
		mLookAt(0, 0, 0), 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent"))
	{
		mCamera = CAD->getSceneManager()->addCameraSceneNode(nullptr, vector3df(XPLICIT_ORIGIN.X, XPLICIT_ORIGIN.Y, XPLICIT_ORIGIN.Z),
			vector3df(0, 5, 0));

		XPLICIT_ASSERT(mCamera);

		mCamera->setName("Camera");

		this->insert("FOV", "90");
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
		ClassComponent::update(class_ptr);

		LocalCameraComponent* self = (LocalCameraComponent*)class_ptr;

        if (!self)
            return;

		auto pos = self->get()->getPosition();

		pos.X = self->index_as_number("Position.X");
		pos.Y = self->index_as_number("Position.Y");
		pos.Z = self->index_as_number("Position.Z");

		self->get()->setPosition(pos);

		self->get()->setFOV(self->index_as_number("FOV"));

		if (self->mLookAt != self->mCamera->getTarget())
		{
			self->mLookAt = self->mCamera->getTarget();

			NetworkPacket packet;

			packet.cmd[XPLICIT_NETWORK_CMD_CAM_POS] = NETWORK_CMD_CAM_POS;

			packet.pos[XPLICIT_NETWORK_X] = self->mLookAt.X;
			packet.pos[XPLICIT_NETWORK_Y] = self->mLookAt.Y;
			packet.pos[XPLICIT_NETWORK_Z] = self->mLookAt.Z;

			packet.channel = XPLICIT_CHANNEL_DATA;

			self->mNetwork->send(packet);
		}
	}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}