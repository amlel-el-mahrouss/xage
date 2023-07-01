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
	class ComponentManager;
	class Component;

	class XPLICIT_API ComponentManager final 
	{
		ComponentManager() = default;

	public:
		~ComponentManager() = default;
		
		XPLICIT_COPY_DEFAULT(ComponentManager);
		
		template <typename T>
		std::vector<T*> all_of(const char* name);
		
		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		bool remove(T* ptr);
		
		template <typename T>
		T* get(const char* name) noexcept;
		
		void update() noexcept;
		
		static ComponentManager* get_singleton_ptr() noexcept;

	private:
		std::vector<Component*> mComponents;

	};

	enum COMPONENT_TYPE : uint8_t
	{
		COMPONENT_HUMANOID, // Humanoid (Player and NPC)
		COMPONENT_LOGIC, // Logic Component
		COMPONENT_CAMERA, // Camera instance
		COMPONENT_SCRIPT, // Script instance (C#, Lua)
		COMPONENT_RENDER, // Graphics Instance (Shader, Particles...)
		COMPONENT_NETWORK, // Network instance
		COMPONENT_PHYSICS, // Physics instance
		COMPONENT_GUI, // GUI instance
		COMPONENT_REPLICATION, // Replicated instance
		COMPONENT_SOUND, // Sound Instance
		COMPONENT_COUNT // the number of Instances type we have here!
	};

	enum PHYSICS_TYPE : uint8_t
	{
		PHYSICS_SIMPLE, /* Rigid-body */
		PHYSICS_COMPLEX, /* Complex Physics, Soft-body */
		PHYSICS_NONE,
		PHYSICS_COUNT,
	};

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