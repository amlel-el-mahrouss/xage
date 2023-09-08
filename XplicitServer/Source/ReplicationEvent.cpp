/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "ReplicationEvent.h"

namespace Xplicit
{
	ReplicationEvent::ReplicationEvent()
		: 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0UL)
	{
	}

	ReplicationEvent::~ReplicationEvent() = default;

	const size_t& ReplicationEvent::size() const noexcept { return mPlayerCount; }

	const char* ReplicationEvent::name() noexcept { return "ReplicationEvent"; }

	void ReplicationEvent::operator()()
	{
		mPlayers = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");

		for (auto& player : mPlayers)
		{
			if (!player->get_peer())
				continue;

			// xasset is a wrapper over http which downloads any contents from play-xplicit.com.
			if (auto replica_xasset = player->get_class()->index_as_string("ReplicaData");
				!replica_xasset.empty())
			{
				mFactory.send(player->get_class()->index_as_number<int>("ReplicaId"), 
					replica_xasset.c_str(),
					player->get_class()->index_as_number<int>("ReplicaType"), player->get_peer()->public_hash);
			
				// you wanna do that

				player->get_class()->insert("ReplicaType", "-1");
				player->get_class()->insert("ReplicaId", "-1");
				player->get_class()->insert("ReplicaData", "nil");
			}
		}
	}
}