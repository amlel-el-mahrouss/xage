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
		EventManager() = default;

	public:
		~EventManager() = default;
		
		EventManager& operator=(const EventManager&) = delete;
		EventManager(const EventManager&) = delete;
		
		template <typename T, typename... Args>
		T* add(Args&&...   args);

		template <typename T>
		bool remove(T* ptr);
		
		template <typename T>
		T* get(const char* name);
		
		void update() noexcept;
		
		static EventManager* get_singleton_ptr() noexcept;

	private:
		std::vector<Event*> mEvents;

	};

	class XPLICIT_API Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		Event& operator=(const Event&) = default;
		Event(const Event&) = default;

		virtual void operator()();

		void update() noexcept;

		bool remove(const EventListener* listener);
		void add(EventListener* listener);

		virtual const char* name() noexcept;

	protected:
		std::vector<EventListener*> mListeners;

		friend EventManager;

	};

	/// <summary>
	/// It listens to a specific event.
	/// For example, PlayerLoginEvent or whatever.
	/// </summary>
	
	class XPLICIT_API EventListener
	{
	public:
		using EventPtr = void*;
		
		EventListener() = default;
		virtual ~EventListener() = default;
		
		EventListener& operator=(const EventListener&) = default;
		EventListener(const EventListener&) = default;
		
		/// <summary>
		/// Update function
		/// </summary>
		/// <param name="env">The Event* class</param>
		 
		virtual void update(EventPtr env) {}
		virtual const char* name() noexcept { return ("EventListener"); }

	};
}

#include "Event.inl"