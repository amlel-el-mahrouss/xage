/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "ComUI.h"
#include "App.h"

#include <Enums.h>
#include <Util.h>
#include <Uri.h>

namespace XPX
{
	LocalReplicationComponent::LocalReplicationComponent(const std::int64_t& hash /* PRIVATE NETWORK HASH */, const String& myId /* XPX LOBBY ID */)
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
		mHash(hash),
		mXpxId(myId)
	{}

	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self ||
			!self->mNetwork)
			return;

		NetworkPacket packet{};
		self->mNetwork->read(packet);

		switch (packet.channel)
		{
		default:
			return;
		}

		// TODO: handle this
	}
}