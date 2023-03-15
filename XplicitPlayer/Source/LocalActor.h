/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.h
 *			Purpose: Client-side Actor Instance
 *
 * =====================================================================
 */

#pragma once

#include <Nplicit.h>
#include <RigidBody.h>

#include "ApplicationContext.h"
#include "NetworkInstance.h"
#include "Camera.h"

// FIXME: rework them when we will roll our own renderer.
namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;
	constexpr const float XPLICIT_SPEED = 20.f;

	class LocalActor : public Instance
	{
	public:
		LocalActor(const int64_t& id);
		virtual ~LocalActor();

		LocalActor& operator=(const LocalActor&) = default;
		LocalActor(const LocalActor&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_ACTOR; }
		virtual const char* name() noexcept override { return ("LocalActor"); }

		virtual void update() override;

	public:
		void attach(CameraInstance* cam) noexcept { m_camera = cam; }

	private:
		NetworkInstance* m_network;
		CameraInstance* m_camera;
		NetworkPacket m_packet;
		int64_t m_id;

	};

	class XPLICIT_API LocalMoveEvent : public Event
	{
	public:
		LocalMoveEvent() = delete;
		explicit LocalMoveEvent(const int64_t& id);
		virtual ~LocalMoveEvent();

		LocalMoveEvent& operator=(const LocalMoveEvent&) = default;
		LocalMoveEvent(const LocalMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		NetworkPacket m_packet;
		int64_t m_id;

	};
}
