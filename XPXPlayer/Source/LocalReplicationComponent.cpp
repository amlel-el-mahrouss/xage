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
#include "MeshComponent.h"
#include "MenuUI.h"
#include "App.h"

#include <CLua.hpp>
#include <Enums.h>
#include <RXML.h>
#include <Util.h>
#include <Uri.h>

namespace XPX
{
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

		if (packet.channel == XPLICIT_CHANNEL_3D)
		{
			auto part = RENDERER->getSceneManager()->getSceneNodeFromId(packet.id);

			if (part)
			{
				auto vec = vector3df(packet.pos[0][XPLICIT_NETWORK_X],
					packet.pos[0][XPLICIT_NETWORK_Y],
					packet.pos[0][XPLICIT_NETWORK_Z]);

				part->setPosition(vec);

				vec = vector3df(packet.pos[1][XPLICIT_NETWORK_X],
					packet.pos[1][XPLICIT_NETWORK_Y],
					packet.pos[1][XPLICIT_NETWORK_Z]);

				part->setRotation(vec);

				vec = vector3df(packet.pos[2][XPLICIT_NETWORK_X],
					packet.pos[2][XPLICIT_NETWORK_Y],
					packet.pos[2][XPLICIT_NETWORK_Z]);

				part->setScale(vec);
			}
			else
			{
				auto part = ComponentSystem::get_singleton_ptr()->add<MeshComponent>(packet.additional_data, packet.id);
			}
		}
	}
}