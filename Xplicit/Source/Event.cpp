/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
	void EventDispatcher::update()
	{
		for (size_t i = 0; i < m_events.size(); i++)
		{
			XPLICIT_ASSERT(m_events[i]);
			m_events[i]->update();
		}
	}

	EventDispatcher* EventDispatcher::get_singleton_ptr()
	{
		static EventDispatcher* ptr = nullptr;

		if (!ptr)
			ptr = new EventDispatcher();

		return ptr;
	}

	void Event::operator()() {}

	void Event::update() 
	{
		for (size_t i = 0; i < m_listeners.size(); i++)
		{
			XPLICIT_ASSERT(m_listeners[i]);
			m_listeners[i]->update(this);
		}

		this->operator()(); 
	}

	const char* Event::name() noexcept { return ("Event"); }

	void Event::add(EventListener* listener) 
	{
		if (listener)
			m_listeners.push_back(listener);
	}

	bool Event::remove(EventListener* listener) 
	{
		if (listener)
		{
			auto it = std::find(m_listeners.cbegin(), m_listeners.cend(), listener);

			if (it != m_listeners.cend())
			{
				m_listeners.erase(it);
				return true;
			}
		}

		return false;
	}
}