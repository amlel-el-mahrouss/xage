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
#include <NetworkComponent.h>
#include "Mesh.h"
#include <GameVar.h>

namespace Xplicit::Player
{
	class LocalPlayerMoveEvent;

	class LocalPlayerComponent : public Component, public StaticMesh
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
		void attach(LocalCameraComponent* cam) noexcept;

	public:
		void set_pos(const vector3df& newPos) noexcept;
		vector3df get_pos() noexcept;

	private:
		IAnimatedMeshSceneNode* mCameraNode;
		GameVarViewPtr mCameraPositionYVar;
		GameVarViewPtr mCameraPositionZVar;
		LocalPlayerMoveEvent* mEvent;
		NetworkComponent* mNetwork;
		LocalCameraComponent* mCam;
		NetworkPacket mPacket;

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
