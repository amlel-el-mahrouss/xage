/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "TimeoutEvent.h"

namespace XPX
{
	TimeoutEvent::TimeoutEvent()
		:
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mCounter(0UL)
	{}

	TimeoutEvent::~TimeoutEvent() = default;

	const char* TimeoutEvent::name() noexcept { return "TimeoutEvent"; }
	
	void TimeoutEvent::operator()()
	{
		for (std::size_t index = 0; index < mNetwork->size(); ++index)
		{
			if (mNetwork->get(index)->status == NETWORK_STAT_DISCONNECTED ||
				mNetwork->get(index)->status == NETWORK_STAT_INVALID ||
				mNetwork->get(index)->status == NETWORK_STAT_STASIS)
				continue;
			
			mNetwork->get(index)->timeout();
		}
	}
}