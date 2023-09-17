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
			node->assign("Anchored", "true");
		}
	}

	const char* NpMovementServerEvent::name() noexcept { return "NpMovementServerEvent"; }

	void NpMovementServerEvent::operator()()
	{
		mDeltaTime = CAD->getTimer()->getTime() - mTimeStamp;
		mTimeStamp = CAD->getTimer()->getTime();

		for (auto* lhsNode : mWorldNodes)
		{
			if (lhsNode->index_as_bool("Anchored"))
				continue;

			for (auto* rhsNode : mWorldNodes)
			{
				if (rhsNode == lhsNode)
					continue;

				if (RigidBodyHelper<NetworkFloat>::is_touching(lhsNode->pos(), lhsNode->scale(), rhsNode->pos(), rhsNode->scale()))
				{
					rhsNode->call_method("Update('Touched')");
					lhsNode->call_method("Update('Touched')");
				}
			}

			auto force = Vector<NetworkFloat>(lhsNode->index_as_number<NplicitFloat>("Force.X"),
				lhsNode->index_as_number<NplicitFloat>("Force.Y"),
				lhsNode->index_as_number<NplicitFloat>("Force.Z"));

			auto velocity = Vector<NetworkFloat>(lhsNode->index_as_number<NplicitFloat>("Force.X") * lhsNode->index_as_number<NplicitFloat>("Weight.X"),
				lhsNode->index_as_number<NplicitFloat>("Force.Y") * lhsNode->index_as_number<NplicitFloat>("Weight.Y"),
				lhsNode->index_as_number<NplicitFloat>("Force.Z") * lhsNode->index_as_number<NplicitFloat>("Weight.Z"));

			lhsNode->pos() = Vector<NetworkFloat>(velocity.X * mDeltaTime, velocity.Y * mDeltaTime, velocity.Z * mDeltaTime);

			lhsNode->assign("DeltaTime", std::to_string(mDeltaTime).c_str());
			lhsNode->call_method("Update('PhysicsFrame')");
		
			if (auto kind = lhsNode->index_as_string("ClassType");
				!kind.empty())
			{
				NetworkPacketRepl repl_packet;

				// Standard RoXML type typechecker.
				if (kind == "Mesh")
				{
					repl_packet.node_kind = 2;
				}
				else if (kind == "Part")
				{
					repl_packet.node_kind = 1;
				}
				else if (kind == "Sphere")
				{
					repl_packet.node_kind = 0;
				}
				else
				{
					continue;
				}

				repl_packet.channel = XPLICIT_CHANNEL_PHYSICS;
				repl_packet.version = XPLICIT_NETWORK_VERSION;

				repl_packet.magic[0] = XPLICIT_NETWORK_MAG_0;
				repl_packet.magic[1] = XPLICIT_NETWORK_MAG_1;
				repl_packet.magic[2] = XPLICIT_NETWORK_MAG_2;

				auto parent = lhsNode->index_as_string("Parent");
				auto name = lhsNode->index_as_string("ClassName");

				memcpy(repl_packet.node_parent, parent.c_str(), parent.size());
				memcpy(repl_packet.node_path, name.c_str(), parent.size());

				NetworkServerContext::send_all(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"),
					(NetworkPacket*)repl_packet);
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