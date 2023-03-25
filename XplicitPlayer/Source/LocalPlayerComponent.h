/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.h
 *			Purpose: Client-side Player
 *
 * =====================================================================
 */

#pragma once


#include <ApplicationContext.h>
#include <NetworkComponent.h>
#include "CameraComponent.h"
#include "MeshComponent.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;

	class LocalPlayerComponent : public Component, public MeshComponent
	{
	public:
		LocalPlayerComponent(const int64_t& public_hash);
		virtual ~LocalPlayerComponent();

		LocalPlayerComponent& operator=(const LocalPlayerComponent&) = default;
		LocalPlayerComponent(const LocalPlayerComponent&) = default;

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;

	public:
		void attach(CameraComponent* cam) noexcept;

	private:
		NetworkComponent* m_network;
		CameraComponent* m_camera;
		NetworkPacket m_packet;
		int64_t m_public_hash;
		int64_t m_then;

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
		NetworkPacket m_packet;
		int64_t m_public_hash;

	};
}
