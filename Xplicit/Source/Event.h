/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Event.h
 *			Purpose: Xplicit Event Dispatcher
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit 
{
	class Event;
	class EventListener;
	class EventDispatcher;

	class XPLICIT_API EventDispatcher final 
	{
	private:
		EventDispatcher() {}

	public:
		~EventDispatcher() {}

		EventDispatcher& operator=(const EventDispatcher&) = delete;
		EventDispatcher(const EventDispatcher&) = delete;

		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		T* get(const char* name);

		template <typename T>
		bool remove(T* name);

	public:
		void update();

	public:
		static EventDispatcher* get_singleton_ptr();

	private:
		std::vector<Event*> m_events;

	};

	class XPLICIT_API Event
	{
	public:
		Event() {}
		virtual ~Event() {}

		Event& operator=(const Event&) = default;
		Event(const Event&) = default;

		virtual void operator()();
		virtual const char* name() noexcept;
		
	public:
		void update();

	};

	// TODO: EventListener
}

#include "Event.inl"