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

namespace Xplicit 
{
	namespace Detail
	{
		struct ComponentAccessor final
		{
			explicit ComponentAccessor()
				: _Pointee(0)
			{}

			std::uintptr_t _Pointee;

			template <typename T>
			T as_type() noexcept
			{
				return reinterpret_cast<T>(_Pointee);
			}
		};
	}

	class XPLICIT_API ComponentManager final 
	{
	public:
		ComponentManager() = default;
		~ComponentManager() = default;
		
	public:
		XPLICIT_COPY_DEFAULT(ComponentManager);
		
	public:
		template <typename T>
		std::vector<T*> all_of(const char* name);
		
		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		bool remove(T* ptr);
		
		template <typename T>
		T* get(const char* name) noexcept;
	
	public:
		void update() noexcept;
		
	public:
		static ComponentManager* get_singleton_ptr() noexcept;

	private:
		std::vector<Detail::ComponentAccessor> mComponents;

	};

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
	* @brief Component Interface, use this class to get all of the needed methods.
	* Last Edit: 7/5/2023
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
		virtual bool should_update() noexcept;
		virtual void update();
		
		virtual COMPONENT_TYPE type() noexcept;
		virtual const char* name() noexcept;

		virtual PHYSICS_TYPE physics() noexcept;
		virtual bool can_collide() noexcept;
		virtual bool has_physics() noexcept;

	};
}

#define XPLICIT_COMPONENT_OVERRIDE()\
		const char* name() noexcept override;\
		COMPONENT_TYPE type() noexcept override;\
		bool should_update() noexcept override;\
		PHYSICS_TYPE physics() noexcept override;




#include "Component.inl"