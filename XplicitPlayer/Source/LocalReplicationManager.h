/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalReplicationManager.h
 *			Purpose: Local replication component.
 *
 * =====================================================================
 */

#pragma once

/* Xplicit root class */
#include <Root.h>

/* Network component */
#include <NetworkComponent.h>

namespace Xplicit
{
	class LocalReplicationManager final : public Component
	{
	public:
		LocalReplicationManager()
			: mNetwork(ComponentManager::get_singleton_ptr()->get_first<NetworkComponent>())
		{}

		~LocalReplicationManager() {}

	public:
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_LOGIC; }
		virtual const char* name() noexcept { return "LocalReplicationManager"; }

		virtual bool should_update() noexcept override { return true; }

		virtual void update() { }

	public:
		XPLICIT_COPY_DEFAULT(LocalReplicationManager);

	private:
		NetworkComponent* mNetwork;

	};
}
