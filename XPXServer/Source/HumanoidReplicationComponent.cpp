/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "HumanoidReplicationComponent.h"

namespace XPX
{
	HumanoidReplicationComponent::HumanoidReplicationComponent()
		: 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0UL)
	{}

	HumanoidReplicationComponent::~HumanoidReplicationComponent() = default;

	const size_t& HumanoidReplicationComponent::size() const noexcept { return mPlayerCount; }

	const char* HumanoidReplicationComponent::name() noexcept { return "HumanoidReplicationComponent"; }

	bool HumanoidReplicationComponent::should_update() noexcept { return true; }

	void HumanoidReplicationComponent::update(ClassPtr ptr)
	{
		HumanoidReplicationComponent* self = (HumanoidReplicationComponent*)ptr;

		self->mPlayers = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>();

		for (auto& player : self->mPlayers)
		{
			if (!player->get_peer())
				continue;

			// xasset is a wrapper over http, it downloads any contents from play-xplicit.com.

			if (auto replica_xasset = player->get_class()->index_as_string("Content");
				!replica_xasset.empty())
			{
				self->mFactory.send(
					player->get_class()->index_as_number<int>("ContentDeliveryKind"),
					replica_xasset.c_str(),
					player->get_class()->index_as_number<int>("ContentKind"), 
					player->get_peer()->public_hash);
			
				// you wanna do that, so we avoid calling this guy again.

				player->get_class()->assign("ContentKind", "-1");
				player->get_class()->assign("ContentDeliveryKind", "-1");
				player->get_class()->assign("Content", "nil");
			}
		}
	}
}