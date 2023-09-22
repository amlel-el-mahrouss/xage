/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NpMovementServerEvent.h"
#include "AABB.h"

#include <NetworkServerComponent.h>
#include <NetworkProtocol.h>
#include <Enums.h>

namespace XPX
{
	static void xpxSendToWorldNode(ClassComponent* node)
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

		repl_packet.pos_fourth[XPLICIT_NETWORK_X] = node->color().R;
		repl_packet.pos_fourth[XPLICIT_NETWORK_Y] = node->color().G;
		repl_packet.pos_fourth[XPLICIT_NETWORK_Z] = node->color().B;
		repl_packet.pos_fourth[XPLICIT_NETWORK_DELTA] = node->alpha();

		repl_packet.pos_third[XPLICIT_NETWORK_X] = node->index_as_number<NetworkFloat>("Rotation.X");
		repl_packet.pos_third[XPLICIT_NETWORK_Y] = node->index_as_number<NetworkFloat>("Rotation.Y");
		repl_packet.pos_third[XPLICIT_NETWORK_Z] = node->index_as_number<NetworkFloat>("Rotation.Z");

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
		for (auto* lhsNode : mWorldNodes)
		{
			if (!lhsNode)
				continue;

			ClassComponent::update(lhsNode);

			if (!lhsNode->index_as_bool("Anchor"))
			{
				for (auto* rhsNode : mWorldNodes)
				{
					if (!rhsNode)
						continue;

					if (rhsNode == lhsNode)
						continue;

					if (AABBHelper::is_touching(lhsNode->pos(), rhsNode->pos()))
					{
						rhsNode->call_method("Update('Touched')");
						lhsNode->call_method("Update('Touching')");

						xpxSendToWorldNode(rhsNode);
						xpxSendToWorldNode(lhsNode);

						break;
					}
				}
			}
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