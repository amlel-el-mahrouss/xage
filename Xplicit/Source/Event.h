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
		explicit Event() = default;
		virtual ~Event() = default;

	public:
		Event& operator=(const Event&) = default;
		Event(const Event&) = default;

	public:
		virtual void operator()();
		virtual const char* name() noexcept;

	public:
		void update() noexcept;

		bool remove(const EventListener* listener);
		void add(EventListener* listener);

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
		
	public:
		explicit EventListener() = default;
		virtual ~EventListener() = default;
		
	public:
		EventListener& operator=(const EventListener&) = default;
		EventListener(const EventListener&) = default;
		
	public: 
		virtual void update(EventPtr env) {}
		virtual const char* name() noexcept { return ("EventListener"); }

	};
}

#include "Event.inl"