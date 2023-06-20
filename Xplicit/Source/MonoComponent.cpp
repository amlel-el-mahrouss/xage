/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: MonoComponent.cpp
 *			Purpose: C# Foundations
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "MonoComponent.h"
#include "HelperMacros.h"
#include "MonoInterop.h"

namespace Xplicit
{
	XPLICIT_API bool xplicit_register_class(MonoString* namespase, MonoString* klass)
	{
		const char* _namespase = mono_string_to_utf8(namespase);
		const char* _klass = mono_string_to_utf8(klass);

		assert(_namespase);
		assert(_klass);

		if (_namespase && _klass)
		{
			ComponentManager::get_singleton_ptr()->add<MonoClassComponent>(_namespase, _klass);
			return true;
		}

		return false;
	}

	static void xplicit_register_events() noexcept
	{
		mono_add_internal_call("XplicitNgine.Engine::RegisterEvent", xplicit_register_event);
		mono_add_internal_call("XplicitNgine.Engine::RegisterClass", xplicit_register_class);
	}

	static std::string mono_to_cxx(MonoString* str)
	{
		// string check
		if (mono_string_length(str) < 1)
			return ("");

		auto chars = mono_string_chars(str);

		std::string _str = { 0 };

		for (size_t i = 0; i < mono_string_length(str); i++)
		{
			_str += chars[i];
		}

		return _str;
	}

	static char* xplicit_read_assembly(const char* path, size_t* _size)
	{
		std::ifstream assembly(path, std::ios::binary | std::ios::ate);

		// no such DLL
		if (!assembly.is_open())
			return nullptr;
	
		std::streampos end = assembly.tellg();
		assembly.seekg(0, std::ios::beg);

		std::streampos size = end - assembly.tellg();

		// empty file.
		if (size == 0)
			return nullptr;

		// finally read our file.
		char* buffer = new char[size];

		assembly.read(buffer, size);
		assembly.close();

		*_size = size;
		return buffer;
	}

	MonoEngineComponent::MonoEngineComponent()
		: m_domain(nullptr), m_app_domain(nullptr)
	{
		XPLICIT_GET_DATA_DIR(data_dir);
		std::string path = data_dir;
		path += "\\Library\\";

		mono_set_assemblies_path(path.c_str());
		m_domain = mono_jit_init_version("XplicitNgine", "v4.0.30319");

		if (m_domain)
		{
#ifndef _NDEBUG
			XPLICIT_INFO("[C#] Domain allocated with success!");
#endif

			m_app_domain = mono_domain_create_appdomain(const_cast<char*>("XplicitScriptRuntime"), nullptr);
			assert(m_app_domain);

			if (!mono_domain_set(m_app_domain, true))
				throw std::runtime_error("MonoDomain: Internal Error");

			// finaly do that, register needed API calls for C#.
			xplicit_register_events();
		}
		else
		{
			throw std::runtime_error("XplicitScriptRuntime: bad domain!");
		}
	}

	MonoEngineComponent::~MonoEngineComponent()
	{
		if (m_domain)
			mono_jit_cleanup(m_domain);
		
		if (m_app_domain)
			mono_jit_cleanup(m_app_domain);
	}

	const char* MonoEngineComponent::name() noexcept { return ("MonoEngineComponent"); }

	COMPONENT_TYPE MonoEngineComponent::type() noexcept { return COMPONENT_LOGIC; }

	void MonoEngineComponent::update() {}

	// checks args
	static bool xplicit_check_args(int argc, const char** argv)
	{
		// arguments check applies here, as we have something in our arguments array.
		if (argc >= 1)
		{
			return argv[0];
		}

		return true;
	}

	// execute a CIL program

	int MonoEngineComponent::run(MonoAssembly* in, int argc, const char** argv)
	{
		if (in)
		{
			if (!xplicit_check_args(argc, argv)) 
				return 1;
			
			return mono_jit_exec(m_app_domain, in, argc, const_cast<char**>(argv));
		}

		return 3;
	}

	// execute a specific method/function
	MonoObject* MonoEngineComponent::run(MonoAssembly* in, const char* fn)
	{
		if (!fn)
			return nullptr;

		if (in)
		{
			MonoImage* img = mono_assembly_get_image(in);
			
			if (!img)
				return nullptr;

			MonoMethodDesc* desc = nullptr;
			desc = mono_method_desc_new(fn, true);

			if (!desc)
				return nullptr;

			MonoMethod* method = mono_method_desc_search_in_image(desc, img);

			MonoObject* obj = nullptr;

			if (method)
				obj = mono_runtime_invoke(method, nullptr, nullptr, nullptr);

			mono_method_desc_free(desc);

			return obj;
		}

		return nullptr;
	}

	// opens a new C# script.
	MonoAssembly* MonoEngineComponent::open(const char* assembly_file)
	{
		if (!assembly_file) return nullptr;
		if (*assembly_file == 0) return nullptr;


		size_t sz = 0;
		// open our assembly file
		char* bytes = xplicit_read_assembly(assembly_file, &sz);

		if (!bytes)
			return nullptr; // return if any failure has been detected.

		MonoImageOpenStatus stat{};
		MonoImage* image = mono_image_open_from_data_full(bytes, sz, true, &stat, false);

		// return if status is bad.
		if (stat != MONO_IMAGE_OK)
		{
#ifndef _NDEBUG
			const char* err = mono_image_strerror(stat);
			XPLICIT_ERROR(err);
#endif

			delete[] bytes;

		}

		MonoAssembly* in_file = mono_assembly_load_from_full(image, assembly_file, &stat, false);
		mono_image_close(image);

		delete[] bytes;

		return in_file;
	}

	MonoClass* MonoEngineComponent::make(Ref<MonoScriptComponent*>& assembly, const char* namespase, const char* klass)
	{
		if (!assembly)
			return nullptr;

		MonoImage* img = mono_assembly_get_image(assembly->m_assembly);
		
		if (!img) 
			return nullptr;

		return mono_class_from_name(img, namespase, klass);
	}

	void MonoEngineComponent::add_internal_call(const char* name, const void* method) noexcept 
	{
		if (!method)
			return;

		if (!name || *name == 0)
			return;

		mono_add_internal_call(name, method);
	}

	MonoDomain* MonoEngineComponent::domain() noexcept { return m_app_domain; }

	// Script Component constructor
	MonoScriptComponent::MonoScriptComponent(const char* filename, bool can_fail)
		: m_filename(filename), m_assembly(nullptr)
	{
		this->m_engine_ref = ComponentManager::get_singleton_ptr()->get<MonoEngineComponent>("MonoEngineComponent");
		assert(this->m_engine_ref);

		// Get the C# assembly.
		m_assembly = this->m_engine_ref->open(this->m_filename.c_str());

		if (!m_assembly && !can_fail)
			throw std::runtime_error("MonoScriptComponent: Couldn't load C# DLL..");
	}

	// destructor, also destroys the EngineComponent, if ref count is below one.
	MonoScriptComponent::~MonoScriptComponent() 
	{
		if (this->m_engine_ref.count() < 1)
			ComponentManager::get_singleton_ptr()->remove<MonoEngineComponent>(this->m_engine_ref.get());
	}

	COMPONENT_TYPE MonoScriptComponent::type() noexcept
	{
		return COMPONENT_SCRIPT;
	}

	const char* MonoScriptComponent::name() noexcept
	{
		return ("MonoScriptComponent");
	}

	void MonoScriptComponent::update()
	{
		// nothing.
	}

	bool MonoScriptComponent::should_update() noexcept { return false; }

	MonoObject* MonoScriptComponent::run(const char* method_name)
	{
		if (!method_name)
			return nullptr; // avoid useless lookups!

		if (this->m_engine_ref)
		{
			assert(m_assembly); // must pass!

			return this->m_engine_ref->run(m_assembly, method_name);
		}

		return nullptr;
	}

	Ref<MonoEngineComponent*>& MonoScriptComponent::get() { return m_engine_ref; }
}