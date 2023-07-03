/*
 * =====================================================================
 * 
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 * 
 * =====================================================================
 */

#pragma once

#include "Event.h"
#include "Xplicit.h"
#include "Component.h"
#include "MonoComponent.h"

namespace Xplicit
{
	class MonoClassComponent;
	class MonoEventListener;
	class MonoEvent;

	class XPLICIT_API MonoClassComponent final : public Component
	{
	public:
		MonoClassComponent() = delete;
		
		MonoClassComponent(const char* namespase, const char* klass);
		~MonoClassComponent() override;

		MonoClassComponent& operator=(const MonoClassComponent&) = default;
		MonoClassComponent(const MonoClassComponent&) = default;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		void update() override;

		bool can_collide() noexcept override;
		bool has_physics() noexcept override;

		bool should_update() noexcept override;

	public:
		operator bool();

	public:
		bool script_should_update() noexcept;
		void script_update() noexcept;

	private:
		Ref<MonoScriptComponent*> m_script; // Image Handle, for the specified Mono app.
		MonoObject* m_object_klass;
		MonoClass* m_klass;

		friend MonoEventListener;

	};

	/// <summary>
	/// Generic Mono Class, written in C++
	/// </summary>
	class XPLICIT_API MonoEvent final : public Event
	{
	public:
		MonoEvent() = delete;
		
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="name">The Event name</param>
		explicit MonoEvent(const char* name);
		~MonoEvent() override = default;

		MonoEvent& operator=(const MonoEvent&) = default;
		MonoEvent(const MonoEvent&) = default;

		void operator()() override;

		const char* name() noexcept override;

	private:
		String m_name;

	};

	/// 
	/// <summary>
	/// MonoEventListener: Inheritable Class
	/// which listens at a MonoEvent.
	/// </summary>
	/// 
	class XPLICIT_API MonoEventListener : public EventListener
	{
	public:
		MonoEventListener() = delete;
		virtual ~MonoEventListener() = default;

	public:
		MonoEventListener(std::string& str);
		MonoEventListener(const char* str);

		MonoEventListener& operator=(const MonoEventListener&) = default;
		MonoEventListener(const MonoEventListener&) = default;

		virtual void update(EventPtr pEvent) override;
		virtual const char* name() noexcept override;

	private:
		std::string m_name;

	};
}
