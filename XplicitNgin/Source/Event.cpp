/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: Event.cpp
 *			Purpose: Xplicit Event Dispatcher and listeners
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Event.h"

namespace Xplicit
{
	void EventSystem::update() noexcept
	{
		if (mEvents.empty())
			return;

		const std::size_t sz = mEvents.size();

		for (std::size_t i = 0; i < sz; i++)
		{
#ifdef XPLICIT_DEBUG
			XPLICIT_ASSERT(mEvents[i]);
#endif // ifdef XPLICIT_DEBUG

			if (mEvents[i])
				mEvents[i]->update();
		}
	}

	EventSystem* EventSystem::get_singleton_ptr() noexcept
	{
		static EventSystem* ptr = nullptr;

		if (!ptr)
			ptr = new EventSystem();

		return ptr;
	}

	void Event::operator()() {}

	void Event::update() noexcept
	{
		this->operator()();
	
		for (size_t i = 0; i < mListeners.size(); i++)
		{
#ifdef XPLICIT_DEBUG
			XPLICIT_ASSERT(mListeners[i]);
#endif // ifdef XPLICIT_DEBUG

			if (mListeners[i])
				mListeners[i]->update(this);
		}
	}

	const char* Event::name() noexcept { return ("Event"); }

	void Event::add(EventListener* listener) 
	{
		if (listener)
			mListeners.push_back(listener);
	}

	bool Event::remove(const EventListener* listener) 
	{
		if (listener)
		{
			const auto it = std::find(mListeners.cbegin(), mListeners.cend(), listener);

			if (it != mListeners.cend())
			{
				mListeners.erase(it);
				return true;
			}
		}

		return false;
	}
}