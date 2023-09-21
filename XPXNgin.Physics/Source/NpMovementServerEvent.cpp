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
		mTimeStamp(CAD->getTimer()->getTime()),
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
		mDeltaTime = CAD->getTimer()->getTime() - mTimeStamp;
		mTimeStamp = CAD->getTimer()->getTime();

		for (auto* lhsNode : mWorldNodes)
		{
			if (!lhsNode)
				continue;

			bool touching = false;

			for (auto* rhsNode : mWorldNodes)
			{
				if (!rhsNode)
					continue;

				if (rhsNode == lhsNode)
					continue;

				if (RigidBodyHelper<NetworkFloat>::is_touching(lhsNode->pos(), lhsNode->scale(),
					rhsNode->pos(), rhsNode->scale()))
				{
					rhsNode->call_method("Update('Touched')");
					lhsNode->call_method("Update('Touched')");

					touching = true;
				}
			}

			if (!lhsNode->index_as_bool("Locked") &&
				!touching)
			{
				auto force = Vector<NetworkFloat>(lhsNode->index_as_number<NplicitFloat>("Force.X"),
					lhsNode->index_as_number<NplicitFloat>("Force.Y"),
					lhsNode->index_as_number<NplicitFloat>("Force.Z"));

				auto velocity = Vector<NetworkFloat>(lhsNode->index_as_number<NplicitFloat>("Force.X") * lhsNode->index_as_number<NplicitFloat>("Weight.X"),
					lhsNode->index_as_number<NplicitFloat>("Force.Y") * lhsNode->index_as_number<NplicitFloat>("Weight.Y"),
					lhsNode->index_as_number<NplicitFloat>("Force.Z") * lhsNode->index_as_number<NplicitFloat>("Weight.Z"));

				lhsNode->pos() = Vector<NetworkFloat>(velocity.X * mDeltaTime, velocity.Y * mDeltaTime, velocity.Z * mDeltaTime);

				lhsNode->assign("DeltaTime", std::to_string(mDeltaTime).c_str());
				lhsNode->call_method("Update('PhysicsProcessDone')");
			}
			
			if (auto kind = lhsNode->index_as_string("ClassType");
				!kind.empty())
			{
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

				repl_packet.pos_third[XPLICIT_NETWORK_X] = lhsNode->color().A;
				
				String fmt = lhsNode->index_as_string("Parent").c_str();

				if (!fmt.empty())
					fmt += ".";
					
				fmt += lhsNode->index_as_string("ClassName").c_str();

				memcpy(repl_packet.additional_data, fmt.c_str(), fmt.size());

				NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
					&repl_packet);
			}
		}
	}

	void NpMovementServerEvent::insert_node(NpSceneNode node)
	{
		if (node)
			mWorldNodes.push_back(node);
	}

	void NpMovementServerEvent::remove_node(NpSceneNode node)
	{
		if (node)
		{
			auto it = std::find(mWorldNodes.cbegin(), mWorldNodes.cend(), node);

			if (it != mWorldNodes.cend())
			{
				mWorldNodes.erase(it);
			}
		}
	}
}