/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: MonoInterop.cpp
 *			Purpose: C# Interop.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "MonoInterop.h"

namespace Xplicit
{
	XPLICIT_API bool xplicit_register_event(MonoString* event_name)
	{
		const char* name = mono_string_to_utf8(event_name);

		if (name)
		{
			EventSystem::get_singleton_ptr()->add<MonoEvent>(name);
			return true;
		}

		return false;
	}

	static MonoObject* xplicit_mono_call(const char* method_name, MonoClass* klass)
	{
		if (!method_name) return nullptr;
		if (!klass) return nullptr;

		MonoMethodDesc* update_desc = mono_method_desc_new(method_name, false);

		if (!update_desc)
			return nullptr;

		MonoMethod* method = mono_method_desc_search_in_class(update_desc, klass);
		mono_method_desc_free(update_desc);

		if (!method)
			return nullptr;

		return mono_runtime_invoke(method, klass, nullptr, nullptr);
	}

	MonoClassComponent::MonoClassComponent(const char* namespase, const char* klass)
		: Component(), m_klass(nullptr), m_object_klass(nullptr), m_script()
	{
		/*
		 * FIXME: Let MonoScriptComponent have it's own name -> get_all<MonoScriptComponent>("MyScript.dll");
		 */

		auto scripts = ComponentSystem::get_singleton_ptr()->all_of<MonoScriptComponent>("MonoScriptComponent");

		for (size_t i = 0; i < scripts.size(); i++)
		{
			m_script = scripts[i];

			m_klass = m_script->get()->make(m_script, namespase, klass);

			if (m_klass)
			{
#ifndef _NDEBUG
				XPLICIT_INFO("[C#] Calling :ctor()..");
#endif
				// search and call the constructor.
				// surronded with assertions.
				m_object_klass = mono_object_new(m_script->get()->domain(), m_klass);
				mono_runtime_object_init(m_object_klass);

				break;
			}
		}
	}

	MonoClassComponent::operator bool() { return m_script; }

	MonoClassComponent::~MonoClassComponent()
	{
		// Free anything that you allocated here.
		// use smart pointers if you can.
	}

	COMPONENT_TYPE MonoClassComponent::type() noexcept
	{
		return COMPONENT_SCRIPT;
	}

	const char* MonoClassComponent::name() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":Name()", m_klass);

		if (obj) 
		{
			return mono_string_to_utf8((MonoString*)obj);
		}

		return ("MonoClassComponent");
	}

	void MonoClassComponent::update(void* class_ptr) {}

	// C# should_update method
	bool MonoClassComponent::script_should_update() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":ShouldUpdate()", m_klass);
		if (obj)
			return *((bool*)mono_object_unbox(obj));

		return false;
	}

	// C# update method.
	void MonoClassComponent::script_update() noexcept
	{
		xplicit_mono_call(":OnUpdate()", m_klass);
	}

	bool MonoClassComponent::can_collide() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":CanCollide()", m_klass);
		if (obj) return *((bool*)mono_object_unbox(obj));

		return false;
	}

	bool MonoClassComponent::has_physics() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":HasPhysics()", m_klass);
		if (obj) return *((bool*)mono_object_unbox(obj));

		return false;
	}

	PHYSICS_TYPE MonoClassComponent::physics() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":Physics()", m_klass);
		if (obj) return *((PHYSICS_TYPE*)mono_object_unbox(obj));

		return PHYSICS_NONE;
	}

	bool MonoClassComponent::should_update() noexcept { return false; }

	MonoEvent::MonoEvent(const char* event_name) 
		: m_name(event_name)
	{
		XPLICIT_ASSERT(event_name);
		XPLICIT_ASSERT(*event_name != 0);

		if (m_name.empty())
			throw EngineError();

	}

	void MonoEvent::operator()()
	{
		if (m_name.empty())
			return;

		auto components = ComponentSystem::get_singleton_ptr()->all_of<MonoClassComponent>(m_name.c_str());

		for (size_t i = 0; i < components.size(); ++i)
		{
			auto* comp = components[i];

			if (!comp)
				continue;

			if (strcmp(comp->name(), m_name.c_str()) == 0)
			{
				if (comp->script_should_update())
					comp->script_update();
			}
		}
	}

	const char* MonoEvent::name() noexcept
	{
		if (m_name.empty())
			return ("MonoEvent");

		return m_name.c_str();
	}

	MonoEventListener::MonoEventListener(std::string& str) : m_name(std::move(str)) {}
	MonoEventListener::MonoEventListener(const char* str) : m_name(str) {}

	void MonoEventListener::update(EventPtr eventPtr)
	{
		(void)eventPtr;

		auto components = ComponentSystem::get_singleton_ptr()->all_of<MonoClassComponent>(m_name.c_str());

		for (size_t i = 0; i < components.size(); ++i)
		{
			auto* comp = components[i];

			if (!comp)
				continue;

			if (strcmp(comp->name(), m_name.c_str()) == 0)
			{
				if (comp->script_should_update())
					comp->script_update();
			}
		}
	}

	const char* MonoEventListener::name() noexcept { return ("EventListener"); }
}
