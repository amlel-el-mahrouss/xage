/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: MonoInterop.cpp
 *			Purpose: C# Classes in C++
 *
 * =====================================================================
 */

#include "MonoInterop.h"

namespace Xplicit
{
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

	MonoClassInstance::MonoClassInstance(const char* namespase, const char* klass)
		: Instance(), m_klass(nullptr), m_object_klass(nullptr), m_script()
	{
		/*
		 * FIXME: Let MonoScriptInstance have it's own name -> get_all<MonoScriptInstance>("MyScript.dll");
		 */

		auto scripts = InstanceManager::get_singleton_ptr()->get_all<MonoScriptInstance>("MonoScriptInstance");

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

	MonoClassInstance::operator bool() { return m_script; }

	MonoClassInstance::~MonoClassInstance()
	{
		// Free anything that you allocated here.
		// use smart pointers if you can.
	}

	MonoClassInstance::INSTANCE_TYPE MonoClassInstance::type() noexcept
	{
		return INSTANCE_SCRIPT;
	}

	const char* MonoClassInstance::name() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":Name()", m_klass);

		if (obj) 
		{
			return mono_string_to_utf8((MonoString*)obj);
		}

		return ("MonoClassInstance");
	}

	void MonoClassInstance::update() 
	{
		// Nothing to do for now.
	}

	// this method actually updates the C# methods.
	void MonoClassInstance::script_update() noexcept
	{
		if (!this->should_update())
			return;

		xplicit_mono_call(":OnUpdate()", m_klass);
	}

	bool MonoClassInstance::can_collide() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":CanCollide()", m_klass);
		if (obj) return *((bool*)mono_object_unbox(obj));

		return false;
	}

	bool MonoClassInstance::has_physics() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":HasPhysics()", m_klass);
		if (obj) return *((bool*)mono_object_unbox(obj));

		return false;
	}

	bool MonoClassInstance::should_update() noexcept
	{
		MonoObject* obj = xplicit_mono_call(":ShouldUpdate()", m_klass);
		if (obj) return *((bool*)mono_object_unbox(obj));

		return false;
	}

	MonoEvent::MonoEvent(const char* event_name) 
		: m_name(event_name)
	{
		if (m_name.empty())
		{
			XPLICIT_CRITICAL("EngineError: MonoEvent::MonoEvent");
			XPLICIT_CRITICAL("EngineError: m_name.empty()");
		}
		else
		{
			XPLICIT_INFO("MonoEvent: Listening at: " + m_name);
		}
	}

	void MonoEvent::operator()()
	{
		if (m_name.empty())
			return;

		auto scripts = InstanceManager::get_singleton_ptr()->get_all<MonoClassInstance>(m_name.c_str());

		for (size_t i = 0; i < scripts.size(); i++)
		{
			if (scripts[i])
				scripts[i]->script_update();
		}
	}

	const char* MonoEvent::name() noexcept
	{
		if (m_name.empty())
			return ("MonoEvent");

		return m_name.c_str();
	}

	XPLICIT_API bool xplicit_register_event(MonoString* event_name)
	{
		const char* name = mono_string_to_utf8(event_name);

		if (name)
		{
			EventDispatcher::get_singleton_ptr()->add<MonoEvent>(name);
			return true;
		}

		return false;
	}
}
