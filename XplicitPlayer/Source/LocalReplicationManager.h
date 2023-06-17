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
	class LocalReplicationManager final
	{
	public:
		LocalReplicationManager()
			: mNetwork(ComponentManager::get_singleton_ptr()->get_first<NetworkComponent>())
		{}

		~LocalReplicationManager() {}

	public:
		XPLICIT_COPY_DEFAULT(LocalReplicationManager);

	private:
		NetworkComponent* mNetwork;

	};
}
