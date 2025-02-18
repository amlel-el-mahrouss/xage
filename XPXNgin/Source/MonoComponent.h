/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: MonoComponent.h
 *			Purpose: C# Foundations
 *
 * =====================================================================
 */

#pragma once

#include "Component.h"
#include "NginCore.h"

#include <jit/jit.h>
#include <metadata/assembly.h>
#include <metadata/debug-helpers.h>

namespace XPX
{
	class MonoScriptComponent;
	class MonoEngineComponent;

	/// <summary>
	/// Mono C# context
	/// </summary>
	class XPLICIT_API MonoEngineComponent final : public Component
	{
	public:
		MonoEngineComponent();
		~MonoEngineComponent() override;

		MonoEngineComponent& operator=(const MonoEngineComponent&) = default;
		MonoEngineComponent(const MonoEngineComponent&) = default;

	public:
		MonoAssembly* open(const char* assembly_file);

		/// <summary>
		/// Runs a function inside of a C# assembly.
		/// </summary>
		/// <param name="in">The imput assembly</param>
		/// <param name="fn">The function</param>
		/// <returns>error code</returns>
		MonoObject* run(MonoAssembly* in, const char* fn);

		/// <summary>
		/// Runs a C# assembly.
		/// </summary>
		/// <param name="in">Input Assembly</param>
		/// <param name="argc">Arg count</param>
		/// <param name="argv">Arg variable.</param>
		/// <returns>error code</returns>
		int run(MonoAssembly* in, int argc, const char** argv);

	public:
		XPLICIT_COMPONENT_OVERRIDE();

	public:
		MonoClass* make(Ref<MonoScriptComponent*>& assembly, const char* namespase, const char* klass);
		void add_internal_call(const char* name, const void* method) noexcept;
		MonoDomain* domain() noexcept;

	private:
		MonoDomain* m_app_domain;
		MonoDomain* m_domain;

	};

	class XPLICIT_API MonoScriptComponent final : public Component
	{
	public:
		MonoScriptComponent(const char* filename, bool can_fail);
		~MonoScriptComponent() override;

		MonoScriptComponent& operator=(const MonoScriptComponent&) = default;
		MonoScriptComponent(const MonoScriptComponent&) = default;

	public:
		XPLICIT_COMPONENT_OVERRIDE();

	public:
		MonoObject* run(const char* method_name);
		Ref<MonoEngineComponent*>& get();

	private:
		Ref<MonoEngineComponent*> m_engine_ref;
		std::basic_string<char> m_filename;
		MonoAssembly* m_assembly;

		friend MonoEngineComponent;

	};

	/* Event and classes */
	XPLICIT_API bool xplicit_register_event(MonoString* event_name);
	XPLICIT_API bool xplicit_register_class(MonoString* namespase, MonoString* klass);
}
