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
	class LocalReplicationComponent final : public Component
	{
	public:
		LocalReplicationComponent()
			: mNetwork(ComponentManager::get_singleton_ptr()->get_first<NetworkComponent>())
		{}

		~LocalReplicationComponent() {}

	public:
		XPLICIT_COPY_DEFAULT(LocalReplicationComponent);

	private:
		NetworkComponent* mNetwork;

	};
}
