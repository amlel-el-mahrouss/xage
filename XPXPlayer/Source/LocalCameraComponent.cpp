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

// fields: FOV, DefaultCameraStyle, Inherits from XPXAttribute.

#include "LocalCameraComponent.h"
#include "UserInterface.h"
#include "App.h"

namespace XPX
{
	LocalCameraComponent::LocalCameraComponent() noexcept
		:
		mCamera(nullptr),
		mLookAt(0, 0, 0), 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent"))
	{
		Renderer::DX11::CameraSystemD3D11* camD3d11 = RENDERER->get().pCamera.get();
		XPLICIT_ASSERT(camD3d11);

		mCamera = camD3d11;
	}

	LocalCameraComponent::~LocalCameraComponent() noexcept = default;

	COMPONENT_TYPE LocalCameraComponent::type() noexcept { return COMPONENT_CAMERA; }

	const char* LocalCameraComponent::name() noexcept { return ("LocalCameraComponent"); }

	void LocalCameraComponent::update(void* class_ptr) 
	{
		LocalCameraComponent* self = (LocalCameraComponent*)class_ptr;

        if (!self)
            return;

		if (self->mLookAt != self->mCamera->rotation())
		{
			self->mLookAt = self->mCamera->rotation();

			NetworkPacket packet;

			packet.cmd[XPLICIT_NETWORK_CMD_TARGET] = NETWORK_CMD_TARGET;

			packet.pos[0][XPLICIT_NETWORK_X] = self->mLookAt.X;
			packet.pos[0][XPLICIT_NETWORK_Y] = self->mLookAt.Y;
			packet.pos[0][XPLICIT_NETWORK_Z] = self->mLookAt.Z;

			self->mNetwork->send(packet);
		}
	}
	
	Renderer::DriverCameraSystem* LocalCameraComponent::get() noexcept { return mCamera; }
}