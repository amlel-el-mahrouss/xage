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
	class LocalHumanoidMoveEvent;

	//! Replicated player component
	/** LocalHumanoidComponent is the player's logic */
	class LocalHumanoidComponent final : public Component, public StaticMesh
	{
	public:
		LocalHumanoidComponent(const int64_t& public_hash);
		~LocalHumanoidComponent() override;

		LocalHumanoidComponent& operator=(const LocalHumanoidComponent&) = default;
		LocalHumanoidComponent(const LocalHumanoidComponent&) = default;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

	public:
		void update() override;

	public:
		void attach(LocalCameraComponent* cam) noexcept;
		vector3df get_pos() noexcept;
		const int64_t& id() noexcept;

	private:
		NetworkComponent* mNetwork;
		LocalCameraComponent* mCam;
		std::int64_t mPublicHash;
		NetworkPacket mPacket;
		vector3df mPos;

	};

	class XPLICIT_API LocalHumanoidMoveEvent : public Event
	{
	public:
		LocalHumanoidMoveEvent() = delete;

	public:
		explicit LocalHumanoidMoveEvent(const std::int64_t& public_hash);
		virtual ~LocalHumanoidMoveEvent();

	public:
		LocalHumanoidMoveEvent& operator=(const LocalHumanoidMoveEvent&) = default;
		LocalHumanoidMoveEvent(const LocalHumanoidMoveEvent&) = default;

	public:
		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		std::int64_t mPublicHash;
		NetworkPacket mPacket;

	};
}
