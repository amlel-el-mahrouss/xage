/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: CharacterHealthMonitorEvent.cpp
 *			Purpose: Spawn and Death handler
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "CharacterHealthMonitorEvent.h"

namespace XPX
{
	constexpr int16_t XPLICIT_DEATH_DELAY = 5;

	/// <summary>
	/// Handle spawn location.
	/// </summary>
	/// <param name="spawner">Spawn component</param>
	/// <param name="humanoid">Target Character</param>
	
	static void XPXHandleSpawn(SpawnComponent* spawner, CharacterComponent* humanoid) noexcept
	{
		if (humanoid && 
			spawner)
		{
			const auto& pos = spawner->get();

			humanoid->pos().X = pos.X;
			humanoid->pos().Y = pos.Y;
			humanoid->pos().Z = pos.Z;
		}
		else
		{
			humanoid->kick("No Spawnpoint.");
		}
	}

	void CharacterHealthMonitorEvent::operator()()
	{
		if (!mNetwork)
			return;

		mSpawner = ComponentSystem::get_singleton_ptr()->get<SpawnComponent>("SpawnComponent");

		if (!mSpawner)
			return;

		auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<CharacterComponent>();

		for (CharacterComponent* humanoid : humanoids)
		{
			if (humanoid == nullptr ||
				humanoid->get_peer() == nullptr)
				continue;
			
			auto* peer_ptr = humanoid->get_peer();

			if (peer_ptr->packet.channel == XPLICIT_CHANNEL_CHAT)
				continue;

			peer_ptr->packet.health = humanoid->get_health();

			if (!humanoid->is_alive() &&
				humanoid->can_spawn())
			{
				humanoid->can_spawn(false);
				humanoid->set_state(HUMANOID_STATE::DEAD);

				for (std::size_t peer = 0UL; peer < humanoids.size(); ++peer)
				{
					peer_ptr->packet.cmd[XPLICIT_NETWORK_CMD_DEAD] = NETWORK_CMD_DEAD;
					peer_ptr->packet.public_hash = humanoid->get_peer()->public_hash;
				}

				Thread respawn_job([](CharacterComponent* humanoid, SpawnComponent* spawner) {
					std::this_thread::sleep_for(std::chrono::seconds(XPLICIT_DEATH_DELAY));

					if (humanoid &&
						spawner &&
						humanoid->get_peer())
					{
						humanoid->can_spawn(true);

						auto humanoids = ComponentSystem::get_singleton_ptr()->all_of<CharacterComponent>();

						for (std::size_t peer = 0UL; peer < humanoids.size(); ++peer)
						{
							if (!humanoids[peer] ||
								!humanoids[peer]->get_peer())
								continue;

							humanoids[peer]->get_peer()->packet.health = XPX_DEFAULT_HEALTH;
							humanoids[peer]->get_peer()->packet.public_hash = humanoid->get_peer()->public_hash;
						}

						XPXHandleSpawn(spawner, humanoid);
					}
				}, humanoid, mSpawner);

				respawn_job.detach();
			}
		}
	}
}