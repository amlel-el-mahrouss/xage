/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Event.inl
 *			Purpose: Xplicit Event System
 *
 * =====================================================================
 */

#include "Avx.h"

template <typename T, typename... Args>
T* Xplicit::EventDispatcher::add(Args&&... args)
{
	T* ptr = new T{ args... };
	XPLICIT_ASSERT(ptr);

	if (ptr)
	{
		m_events.push_back(reinterpret_cast<Event*>(ptr));
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::EventDispatcher::get(const char* name)
{
	for (size_t i = 0; i < m_events.size(); i++)
	{
		if (!m_events[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, m_events[i]->name()))
#else
		if (strcmp(name, m_events[i]->name()) == 0)
#endif
			return static_cast<T*>(m_events[i]);
	}

	return nullptr;
}

template <typename T>
bool Xplicit::EventDispatcher::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find(m_events.cbegin(), m_events.cend(), ptr);

	if (iterator != m_events.cend())
	{
		m_events.erase(iterator);
		delete ptr;

		return true;
	}

	return false;
}