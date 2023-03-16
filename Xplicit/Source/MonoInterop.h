/*
 * =====================================================================
 * 
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 * 
 *			File: MonoInterop.h
 *			Purpose: C# Classes in C++
 * 
 * =====================================================================
 */

#pragma once

#include "Event.h"
#include "Instance.h"
#include "Xplicit.h"
#include "MonoInstance.h"

namespace Xplicit
{
	class XPLICIT_API MonoClassInstance final : public Instance
	{
	public:
		MonoClassInstance() = delete;

	public:
		MonoClassInstance(const char* namespase, const char* klass);
		virtual ~MonoClassInstance();

		MonoClassInstance& operator=(const MonoClassInstance&) = default;
		MonoClassInstance(const MonoClassInstance&) = default;

		virtual INSTANCE_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual void update() override;
		void script_update() noexcept;

		virtual bool can_collide() noexcept override;
		virtual bool has_physics() noexcept override;

		virtual bool should_update() noexcept override;

		operator bool();

	private:
		Ref<MonoScriptInstance*> m_script; // Image Handle, for the specified Mono app.
		MonoObject* m_object_klass;
		MonoClass* m_klass;

	};

	class XPLICIT_API MonoEvent final : public Event
	{
	public:
		MonoEvent() = delete;

	public:
		MonoEvent(const char* event_name);
		virtual ~MonoEvent() = default;

		MonoEvent& operator=(const MonoEvent&) = default;
		MonoEvent(const MonoEvent&) = default;

		virtual void operator()() override;

		virtual const char* name() noexcept override;

	private:
		std::string m_name;

	};
}
