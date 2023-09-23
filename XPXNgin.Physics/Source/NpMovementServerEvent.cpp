/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 * 
 *			Purpose: Built-in Physics engine for XPX tech.
 *
 * =====================================================================
 */

#include "NpMovementServerEvent.h"

#include <NetworkServerComponent.h>
#include <NetworkProtocol.h>
#include <Enums.h>

namespace XPX
{
	static void xpxSendToClient(ClassComponent* node)
	{
		NetworkPacket repl_packet{};

		repl_packet.channel = XPLICIT_CHANNEL_PHYSICS;
		repl_packet.version = XPLICIT_NETWORK_VERSION;

		repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
		repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
		repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

		repl_packet.pos[XPLICIT_NETWORK_X] = node->pos().X;
		repl_packet.pos[XPLICIT_NETWORK_Y] = node->pos().Y;
		repl_packet.pos[XPLICIT_NETWORK_Z] = node->pos().Z;

		repl_packet.pos_second[XPLICIT_NETWORK_X] = node->scale().X;
		repl_packet.pos_second[XPLICIT_NETWORK_Y] = node->scale().Y;
		repl_packet.pos_second[XPLICIT_NETWORK_Z] = node->scale().Z;

		repl_packet.pos_third[XPLICIT_NETWORK_X] = node->rotation().X;
		repl_packet.pos_third[XPLICIT_NETWORK_Y] = node->rotation().Y;
		repl_packet.pos_third[XPLICIT_NETWORK_Z] = node->rotation().Z;

		String fmt = node->index_as_string("Parent").c_str();

		if (!fmt.empty())
			fmt += ".";

		fmt += node->index_as_string("ClassName").c_str();

		memcpy(repl_packet.additional_data, fmt.c_str(), fmt.size());

		NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
			&repl_packet);
	}

	NpMovementServerEvent::NpMovementServerEvent() noexcept : mWorldNodes() {}
	NpMovementServerEvent::~NpMovementServerEvent() noexcept = default;

	const char* NpMovementServerEvent::name() noexcept { return "NpMovementServerEvent"; }

	void NpMovementServerEvent::operator()()
	{
		for (auto* node : mWorldNodes)
		{
			if (!node)
				continue;

			ClassComponent::update((ClassPtr)node);

			xpxSendToClient(node);
		}
	}

	bool NpMovementServerEvent::insert_node(NpSceneNode node)
	{
		if (node)
		{
			mWorldNodes.push_back(node);
			return true;
		}

		return false;
	}

	bool NpMovementServerEvent::remove_node(NpSceneNode node)
	{
		if (node)
		{
			auto it = std::find(mWorldNodes.cbegin(), mWorldNodes.cend(), node);

			if (it != mWorldNodes.cend())
			{
				mWorldNodes.erase(it);
				return true;
			}
		}

		return false;
	}
}