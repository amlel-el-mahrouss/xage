/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

 //! This file handles the engine component system stuff.

namespace Xplicit
{
	enum COMPONENT_TYPE : uint8_t
	{
		COMPONENT_HUMANOID, // Humanoid (Player and NPC)
		COMPONENT_LOGIC, // Logic Component
		COMPONENT_CAMERA, // Camera component
		COMPONENT_SCRIPT, // Script component (C#, Lua)
		COMPONENT_RENDER, // Graphics component (Shader, Particles...)
		COMPONENT_NETWORK, // Network component
		COMPONENT_PHYSICS, // Physics component
		COMPONENT_GUI, // GUI component
		COMPONENT_REPLICATION, // Replicated component
		COMPONENT_SOUND, // Sound component
		COMPONENT_GENERIC,
		COMPONENT_COUNT // the number of components type we have registered here!
	};

	enum PHYSICS_TYPE : uint8_t
	{
		PHYSICS_SIMPLE, /* Rigid-body */
		PHYSICS_COMPLEX, /* Complex Physics, Soft-body */
		PHYSICS_NONE,
		PHYSICS_COUNT,
	};

	/*
	* ----------------------------------------------------------------------------
	* @name Component
	* @brief C++ interface.
	* ----------------------------------------------------------------------------
	*/

	class XPLICIT_API Component
	{
	public:
		Component();
		virtual ~Component();

	public:
		XPLICIT_COPY_DEFAULT(Component);

	public:
		virtual PHYSICS_TYPE physics() noexcept;
		virtual COMPONENT_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual bool can_collide() noexcept;
		virtual bool has_physics() noexcept;

	};

	//! Component Class Ptr
	typedef void* ClassPtr;

	namespace Details
	{
		typedef void* ComponentData;

		typedef void(__cdecl *ComponentUpdateAccessor)(void*);
		typedef bool(__cdecl *ComponentUpdateEvalAccessor)(void);

		struct ComponentAccessor final
		{
		public:
			explicit ComponentAccessor()
				: _Pointee(nullptr),
				  _Update(nullptr),
				  _Eval(nullptr)
			{}

		public:
			ComponentUpdateEvalAccessor _Eval;
			ComponentUpdateAccessor _Update;
			ComponentData _Pointee;
			String _Name;

		public:
			template <typename Y>
			Y as_type() noexcept
			{
				return reinterpret_cast<Y>(_Pointee);
			}
		};
	}

	class XPLICIT_API ComponentSystem final
	{
		explicit ComponentSystem() = default;

	public:
		~ComponentSystem() = default;

	public:
		XPLICIT_COPY_DELETE(ComponentSystem);

	public:
		template <typename T>
		std::vector<T*> all_of(const char* name);

	public:
		template <typename T, typename... Args>
		T* add(Args&&... args);

	public:
		template <typename T>
		bool remove(T* ptr);

	public:
		template <typename T>
		T* get(const char* name);

	public:
		void update();

	public:
		static ComponentSystem* get_singleton_ptr() noexcept;

	private:
		std::vector<Details::ComponentAccessor> mComponents;

	};
}

#define XPLICIT_COMPONENT_OVERRIDE()\
		const char* name() noexcept override;\
		COMPONENT_TYPE type() noexcept override;\
		static bool should_update() noexcept;\
		static void update(void* class_ptr);\
		PHYSICS_TYPE physics() noexcept override;\
		bool has_physics() noexcept override;\
		bool can_collide() noexcept override;


#include "Component.inl"