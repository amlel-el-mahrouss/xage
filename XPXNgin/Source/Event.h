/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

namespace XPX 
{
	///! do that so that we don't deal with any errors.
	class EventListener;
	class EventSystem;
	class Event;
	
	class XPLICIT_API EventSystem final 
	{
		explicit EventSystem() = default;

	public:
		~EventSystem() = default;
		
	public:
		XPLICIT_COPY_DELETE(EventSystem);

	public:
		template <typename T, typename... Args>
		T* add(Args&&...   args);

		template <typename T>
		bool remove(T* ptr);
		
		template <typename T>
		T* get(const char* name);
		
	public:
		void update() noexcept;
		
	public:
		static EventSystem* get_singleton_ptr() noexcept;

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
		virtual const char* name() noexcept;
		virtual void operator()();

	public:
		void update() noexcept;

		bool remove(const EventListener* listener);
		void add(EventListener* listener);

	protected:
		std::vector<EventListener*> mListeners;

		friend EventSystem;

	};

	/// <summary>
	/// It listens to a specific event.
	/// For example, LoginEvent or whatever.
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