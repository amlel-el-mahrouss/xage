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
#include "App.h"
#include "MenuUI.h"
#include "ClientUtils.h"

namespace XPX
{
	LocalCameraComponent::LocalCameraComponent() noexcept
		: ClassComponent(XPLICIT_ORIGIN, Vector<NetworkFloat>(1, 1, 1), Color<NetworkFloat>(0, 0, 0), nullptr, XPLICIT_LUA_NAMESPACE, "Camera"),
		mCamera(nullptr),
		mLookAt(0, 0, 0), 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent"))
	{
		mCamera = CAD->getSceneManager()->addCameraSceneNode(nullptr);
		CAD->getSceneManager()->setActiveCamera(mCamera);
		
		mLight = CAD->getSceneManager()->addLightSceneNode(mCamera, core::vector3df(0, 0, 0),
		video::SColorf(1.f, 1.0f, 1.0f, 1.0f), 1000.0f);

		XPLICIT_ASSERT(mCamera);
		XPLICIT_ASSERT(mLight);

		mCamera->setPosition(vector3df(XPLICIT_ORIGIN.X, XPLICIT_ORIGIN.Y, XPLICIT_ORIGIN.Z));
		mCamera->setName("Camera");

		this->insert("FOV", std::to_string(mCamera->getFOV()));
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

		self->mLight->setParent(self->mCamera);

		if (self->index_as_number("FOV") != self->get()->getFOV())
			self->get()->setFOV(self->index_as_number("FOV"));

		if (self->mLookAt != self->mCamera->getTarget())
		{
			self->mLookAt = self->mCamera->getTarget();

			NetworkPacket packet;

			packet.cmd[XPLICIT_NETWORK_CMD_TARGET] = NETWORK_CMD_TARGET;

			packet.pos[XPLICIT_NETWORK_X] = self->mLookAt.X;
			packet.pos[XPLICIT_NETWORK_Y] = self->mLookAt.Y;
			packet.pos[XPLICIT_NETWORK_Z] = self->mLookAt.Z;

			self->mNetwork->set_channel(XPLICIT_CHANNEL_DATA);

			self->mNetwork->send(packet);
		}
	}

	irr::scene::ICameraSceneNode* LocalCameraComponent::get() noexcept { return mCamera; }
}