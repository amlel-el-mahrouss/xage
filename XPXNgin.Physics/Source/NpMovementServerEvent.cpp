/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NpMovementServerEvent.h"

#include <NetworkServerComponent.h>
#include <NetworkProtocol.h>
#include <Enums.h>

namespace XPX
{
	NpMovementServerEvent::NpMovementServerEvent() noexcept
		: 
		mWorldNodes(),
		mTimeStamp(2),
		mDeltaTime(0.0)
	{}

	NpMovementServerEvent::~NpMovementServerEvent() noexcept
	{
		for (auto* node : mWorldNodes)
		{
			node->assign("Locked", "true");
		}
	}

	const char* NpMovementServerEvent::name() noexcept { return "NpMovementServerEvent"; }

	void NpMovementServerEvent::operator()()
	{
		mDeltaTime = mTimeStamp;
		++mDeltaTime;

		for (auto* lhsNode : mWorldNodes)
		{
			if (!lhsNode ||
				lhsNode->index_as_bool("Anchored"))
				continue;

			lhsNode->assign("DeltaTime", std::to_string(mDeltaTime).c_str());
			
			if (!lhsNode->index_as_bool("Anchored"))
			{
				for (auto* rhsNode : mWorldNodes)
				{
					if (!rhsNode)
						return;

					if (rhsNode == lhsNode ||
						rhsNode->pos() == lhsNode->pos())
						continue;

					auto x1 = lhsNode->pos().X;
					auto x2 = lhsNode->scale().X;

					auto rx1 = rhsNode->pos().X;
					auto rx2 = rhsNode->scale().X;

					if (auto where = (x1 / x2) * (rx1 / rx2);
						where <= (rx1 / rx2))
					{
						auto res = Vector<NplicitFloat>((x1 + rx1) / where, (x2 + rx2) / where, (x1 + x2) / where);
						lhsNode->pos() = res;

						if (rhsNode->index_as_bool("Anchored"))
						{
							lhsNode->pos() = res;

							if (auto formula = lhsNode->pos().X * where;
								formula > lhsNode->pos().X)
								lhsNode->pos().X += formula;

							if (auto formula = lhsNode->pos().Y * where;
								formula > lhsNode->pos().Y)
								lhsNode->pos().Y += formula;

							if (auto formula = lhsNode->pos().Z * where;
								formula > lhsNode->pos().Z)
								lhsNode->pos().Z += formula;
						}

						rhsNode->call_method("Update('Touched')");
						lhsNode->call_method("Update('Touched')");

						continue;
					}

					lhsNode->pos().Y -= mDeltaTime;
				}
			}

			NetworkPacket repl_packet{};

			repl_packet.channel = XPLICIT_CHANNEL_PHYSICS;
			repl_packet.version = XPLICIT_NETWORK_VERSION;

			repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
			repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
			repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

			repl_packet.pos[XPLICIT_NETWORK_X] = lhsNode->pos().X;
			repl_packet.pos[XPLICIT_NETWORK_Y] = lhsNode->pos().Y;
			repl_packet.pos[XPLICIT_NETWORK_Z] = lhsNode->pos().Z;

			repl_packet.pos_second[XPLICIT_NETWORK_X] = lhsNode->scale().X;
			repl_packet.pos_second[XPLICIT_NETWORK_Y] = lhsNode->scale().Y;
			repl_packet.pos_second[XPLICIT_NETWORK_Z] = lhsNode->scale().Z;

			repl_packet.pos_fourth[XPLICIT_NETWORK_X] = lhsNode->color().R;
			repl_packet.pos_fourth[XPLICIT_NETWORK_Y] = lhsNode->color().G;
			repl_packet.pos_fourth[XPLICIT_NETWORK_Z] = lhsNode->color().B;

			repl_packet.pos_third[XPLICIT_NETWORK_X] = lhsNode->alpha();

			String fmt = lhsNode->index_as_string("Parent").c_str();

			if (!fmt.empty())
				fmt += ".";

			fmt += lhsNode->index_as_string("ClassName").c_str();

			memcpy(repl_packet.additional_data, fmt.c_str(), fmt.size());

			NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
				&repl_packet);
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