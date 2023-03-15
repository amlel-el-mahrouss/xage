/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: PlayerJoinLeaveEvent.h
 *			Purpose: Player Movements.
 *
 * =====================================================================
 */

#include "PlayerPhysicsEvent.h"

namespace Xplicit
{
	void PlayerPhysicsEvent::operator()()
	{
		auto actors = InstanceManager::get_singleton_ptr()->all_of<Actor>("Actor");

		for (Actor* actor : actors)
		{
			if (!actor->get())
				continue;

			if (actor->health() <= 0)
				continue;

			// Todo: NPlicitNgin Physics
		}
	}
}