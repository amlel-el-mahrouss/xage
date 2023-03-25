/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Event.h
 *			Purpose: Xplicit Event Dispatcher and listeners
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
		EventDispatcher() = default;

	public:
		~EventDispatcher() = default;

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
		void update() noexcept;

	public:
		void add(EventListener* listener);
		bool remove(EventListener* listener);

	public:
		virtual const char* name() noexcept;

	protected:
		std::vector<EventListener*> m_listeners;

		friend EventDispatcher;

	};

	/// <summary>
	/// It listens to a specific event.
	/// For example, PlayerJoinEvent or whatever.
	/// </summary>
	class XPLICIT_API EventListener
	{
	public:
		using EventPtr = void*;

	public:
		EventListener() = default;
		virtual ~EventListener() = default;

		EventListener& operator=(const EventListener&) = default;
		EventListener(const EventListener&) = default;

		/// <summary>
		/// Update function
		/// </summary>
		/// <param name="pEvent">The Event* class</param>
		virtual void update(EventPtr ptr_event) {}
		virtual const char* name() noexcept { return ("EventListener"); }

	};
}

#include "Event.inl"