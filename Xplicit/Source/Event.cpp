/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
	void EventManager::update() noexcept
	{
		for (std::size_t i = 0; i < mEvents.size(); i++)
		{
			XPLICIT_ASSERT(mEvents[i]);
			mEvents[i]->update();
		}
	}

	EventManager* EventManager::get_singleton_ptr() noexcept
	{
		static EventManager* ptr = nullptr;

		if (!ptr)
			ptr = new EventManager();

		return ptr;
	}

	void Event::operator()() {}

	void Event::update() noexcept
	{
		for (size_t i = 0; i < mListeners.size(); i++)
		{
			XPLICIT_ASSERT(mListeners[i]);
			mListeners[i]->update(this);
		}

		this->operator()();
	}

	const char* Event::name() noexcept { return ("Event"); }

	void Event::add(EventListener* listener) 
	{
		if (listener)
			mListeners.push_back(listener);
	}

	bool Event::remove(EventListener* listener) 
	{
		if (listener)
		{
			auto it = std::find(mListeners.cbegin(), mListeners.cend(), listener);

			if (it != mListeners.cend())
			{
				mListeners.erase(it);
				return true;
			}
		}

		return false;
	}
}