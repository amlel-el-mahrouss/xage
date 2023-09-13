/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "NpMovementSharedEvent.h"

namespace XPX
{
	NpMovementSharedEvent::NpMovementSharedEvent() noexcept
		: 
		mWorldNodes(),
		mTimeStamp(CAD->getTimer()->getTime()),
		mDeltaTime(0.0)
	{}

	NpMovementSharedEvent::~NpMovementSharedEvent() noexcept
	{
		for (auto* node : mWorldNodes)
		{
			node->assign("Anchored", "true");
		}
	}

	const char* NpMovementSharedEvent::name() noexcept { return "NpMovementSharedEvent"; }

	void NpMovementSharedEvent::operator()()
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

				if (RigidBodyHelper<NplicitFloat>::is_touching(lhsNode->pos(), lhsNode->scale(), rhsNode->pos(), rhsNode->scale()))
				{
					rhsNode->call_method("Update('Touched')");
					lhsNode->call_method("Update('Touched')");
				}
			}

			auto force = Vector<float>(lhsNode->index_as_number<NplicitFloat>("Force.X"),
				lhsNode->index_as_number<NplicitFloat>("Force.Y"),
				lhsNode->index_as_number<NplicitFloat>("Force.Z"));

			auto velocity = Vector<float>(lhsNode->index_as_number<NplicitFloat>("Force.X") * lhsNode->index_as_number<NplicitFloat>("Weight.X"),
				lhsNode->index_as_number<NplicitFloat>("Force.Y") * lhsNode->index_as_number<NplicitFloat>("Weight.Y"),
				lhsNode->index_as_number<NplicitFloat>("Force.Z") * lhsNode->index_as_number<NplicitFloat>("Weight.Z"));

			lhsNode->pos() = Vector<float>(velocity.X * mDeltaTime, velocity.Y * mDeltaTime, velocity.Z * mDeltaTime);

			lhsNode->assign("DeltaTime", std::to_string(mDeltaTime).c_str());
			lhsNode->call_method("Update('PhysicsChanged')");
		}
	}

	void NpMovementSharedEvent::insert_node(NpSceneNode node)
	{
		if (node)
			mWorldNodes.push_back(node);
	}
}