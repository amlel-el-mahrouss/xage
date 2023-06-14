/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalActor.h
 *			Purpose: Client-side Player
 *
 * =====================================================================
 */

#pragma once

#include <NetworkComponent.h>

#include "CameraComponent.h"
#include "MeshComponent.h"

namespace Xplicit::Player
{
	class LocalMoveEvent;

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

		const int64_t& id() noexcept;

	public:
		virtual void update() override;

	public:
		void attach(CameraComponent* cam) noexcept;

	public:
		void set_pos(const vector3df& newPos) noexcept;
		vector3df get_pos() noexcept;

	private:
		IAnimatedMeshSceneNode* mCameraNode;
		NetworkComponent* mNetwork;
		CameraComponent* mCam;
		NetworkPacket mPacket;
		LocalMoveEvent* mEvent;

	private:
		std::int64_t mPublicHash;

	};

	class XPLICIT_API LocalMoveEvent : public Event
	{
	public:
		LocalMoveEvent() = delete;

	public:
		explicit LocalMoveEvent(const std::int64_t& public_hash);
		virtual ~LocalMoveEvent();

		LocalMoveEvent& operator=(const LocalMoveEvent&) = default;
		LocalMoveEvent(const LocalMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		NetworkPacket mPacket;

	private:
		std::int64_t mPublicHash;
		std::size_t mCooldown;

	};
}
