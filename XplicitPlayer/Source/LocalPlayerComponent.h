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

#include "LocalCameraComponent.h"
#include "Mesh.h"

#include <NetworkComponent.h>
#include <GameVar.h>

namespace Xplicit::Player
{
	class LocalPlayerMoveEvent;

	//! Replicated player component
	/** LocalPlayerComponent is the player's logic */
	class LocalPlayerComponent : public Component, public StaticMesh
	{
	public:
		LocalPlayerComponent(const int64_t& public_hash);
		virtual ~LocalPlayerComponent();

		LocalPlayerComponent& operator=(const LocalPlayerComponent&) = default;
		LocalPlayerComponent(const LocalPlayerComponent&) = default;

		virtual COMPONENT_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

	public:
		virtual void update() override;

	public:
		void attach(LocalCameraComponent* cam) noexcept;
		vector3df get_pos() noexcept;
		const int64_t& id() noexcept;

	private:
		NetworkComponent* mNetwork;
		LocalCameraComponent* mCam;
		NetworkPacket mPacket;
		vector3df mPos;

	private:
		std::int64_t mPublicHash;

	};

	class XPLICIT_API LocalPlayerMoveEvent : public Event
	{
	public:
		LocalPlayerMoveEvent() = delete;

	public:
		explicit LocalPlayerMoveEvent(const std::int64_t& public_hash);
		virtual ~LocalPlayerMoveEvent();

	public:
		LocalPlayerMoveEvent& operator=(const LocalPlayerMoveEvent&) = default;
		LocalPlayerMoveEvent(const LocalPlayerMoveEvent&) = default;

	public:
		virtual void operator()() override;
		const char* name() noexcept;

	private:
		std::int64_t mPublicHash;

	private:
		NetworkComponent* mNetwork;
		NetworkPacket mPacket;

	};
}
