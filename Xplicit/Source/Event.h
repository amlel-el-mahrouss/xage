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

	public:
		void push_back(EventListener* listener);
		bool remove(EventListener* listener);

	private:
		std::vector<EventListener*> m_listeners;

	};

	// TODO: EventListener

	class XPLICIT_API EventListener
	{
	public:
		using EventTypePtr = void*;

	public:
		EventListener() = default;
		virtual ~EventListener() = default;

		EventListener& operator=(const EventListener&) = default;
		EventListener(const EventListener&) = default;

		virtual void update(EventTypePtr pEvent) = 0;

	};
}

#include "Event.inl"