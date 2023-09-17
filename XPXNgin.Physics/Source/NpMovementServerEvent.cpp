/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NpMovementServerEvent.h"
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