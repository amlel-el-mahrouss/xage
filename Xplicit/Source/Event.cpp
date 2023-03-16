/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Event.cpp
 *			Purpose: Xplicit Event System
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

	void Event::update() { this->operator()(); }

	const char* Event::name() noexcept { return ("Event"); }
}