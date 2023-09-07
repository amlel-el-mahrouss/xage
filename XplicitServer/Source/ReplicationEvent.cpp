/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
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
		mPlayers = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");
	}

	ReplicationEvent::~ReplicationEvent() = default;

	const size_t& ReplicationEvent::size() const noexcept { return mPlayerCount; }

	const char* ReplicationEvent::name() noexcept { return "ReplicationEvent"; }

	void ReplicationEvent::operator()()
	{
		for (auto& player : mPlayers)
		{
			if (auto xasset = player->get_class()->index_as_string("ReplicatedData");
				!xasset.empty())
			{
				mFactory.send(player->get_class()->index_as_number<int>("ReplicatedDataId"), 
					xasset.c_str(),
					player->get_class()->index_as_number<int>("ReplicatedDataType"), player->get_peer()->public_hash);
			
				// you wanna do that

				player->get_class()->insert("ReplicatedDataType", "-1");
				player->get_class()->insert("ReplicatedDataId", "-1");
				player->get_class()->insert("ReplicatedData", "nil");
			}
		}
	}
}