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

#include "LocalReplicationComponent.h"

namespace Xplicit::Player
{
	LocalReplicationComponent::INSTANCE_TYPE LocalReplicationComponent::type() noexcept { return INSTANCE_LOGIC; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update()
	{
		XPLICIT_ASSERT(mNetwork);

		auto packet = mNetwork->get();

		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

	}
}