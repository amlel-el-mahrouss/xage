/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit 
{
	class EventListener;
	class EventManager;
	class Event;
	
	class XPLICIT_API EventManager final 
	{
	private:
		EventManager() = default;

	public:
		~EventManager() = default;

	public:
		EventManager& operator=(const EventManager&) = delete;
		EventManager(const EventManager&) = delete;

	public:
		template <typename T, typename... Args>
		T* add(Args&&...   args);

		template <typename T>
		bool remove(T* ptr);

	public:
		template <typename T>
		T* get(const char* name);

	public:
		void update() noexcept;

	public:
		static EventManager* get_singleton_ptr() noexcept;

	private:
		std::vector<Event*> mEvents;

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

		bool remove(EventListener* listener);
		void add(EventListener* listener);

		virtual const char* name() noexcept;

	protected:
		std::vector<EventListener*> mListeners;

		friend EventManager;

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

	public:
		EventListener& operator=(const EventListener&) = default;
		EventListener(const EventListener&) = default;

	public:
		/// <summary>
		/// Update function
		/// </summary>
		/// <param name="pEvent">The Event* class</param>
		 
		virtual void update(EventPtr ptr_event) {}
		virtual const char* name() noexcept { return ("EventListener"); }

	};
}

#include "Event.inl"