/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.h
 *			Purpose: Client-side Actors
 *
 * =====================================================================
 */

#pragma once

#include <Nplicit.h>
#include <RigidBody.h>

#include "ApplicationContext.h"
#include "NetworkComponent.h"
#include "MeshComponent.h"
#include "Camera.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;
	constexpr const float XPLICIT_SPEED = 20.f;

	class LocalActor : public Component, public MeshComponent
	{
	public:
		LocalActor(const int64_t& public_hash);
		virtual ~LocalActor();

		LocalActor& operator=(const LocalActor&) = default;
		LocalActor(const LocalActor&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_ACTOR; }
		virtual const char* name() noexcept override { return ("LocalActor"); }

		virtual void update() override;

	public:
		void attach(CameraComponent* cam) noexcept;

	private:
		NetworkComponent* m_network;
		CameraComponent* m_camera;
		NetworkPacketHeader m_packet;
		int64_t m_public_hash;

	};

	class XPLICIT_API LocalMoveEvent : public Event
	{
	public:
		LocalMoveEvent() = delete;

	public:
		explicit LocalMoveEvent(const int64_t& public_hash);
		virtual ~LocalMoveEvent();

		LocalMoveEvent& operator=(const LocalMoveEvent&) = default;
		LocalMoveEvent(const LocalMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* m_network;
		NetworkPacketHeader m_packet;
		int64_t m_public_hash;

	};
}
