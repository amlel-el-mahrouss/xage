/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "HumanoidReplicationEvent.h"

namespace XPX
{
	HumanoidReplicationEvent::HumanoidReplicationEvent()
		: 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0UL)
	{
	}

	HumanoidReplicationEvent::~HumanoidReplicationEvent() = default;

	const size_t& HumanoidReplicationEvent::size() const noexcept { return mPlayerCount; }

	const char* HumanoidReplicationEvent::name() noexcept { return "HumanoidReplicationEvent"; }

	void HumanoidReplicationEvent::operator()()
	{
		mPlayers = ComponentSystem::get_singleton_ptr()->all_of<HumanoidComponent>("HumanoidComponent");

		for (auto& player : mPlayers)
		{
			if (!player->get_peer())
				continue;

			// xasset is a wrapper over http which downloads any contents from play-xplicit.com.

			if (auto replica_xasset = player->get_class()->index_as_string("PacketData");
				!replica_xasset.empty())
			{
				mFactory.send(player->get_class()->index_as_number<int>("PacketContentKind"), 
					replica_xasset.c_str(),
					player->get_class()->index_as_number<int>("PacketKind"), 
					player->get_peer()->public_hash);
			
				// you wanna do that

				player->get_class()->assign("PacketKind", "-1");
				player->get_class()->assign("PacketContentKind", "-1");
				player->get_class()->assign("PacketData", "nil");
			}
		}
	}
}