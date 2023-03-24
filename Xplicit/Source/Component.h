/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Component.h
 *			Purpose: Xplicit's Component System
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit 
{
	class Component;
	class ComponentManager;

	class XPLICIT_API ComponentManager final 
	{
	private:
		ComponentManager() = default;

	public:
		~ComponentManager() = default;

		ComponentManager& operator=(const ComponentManager&) = delete;
		ComponentManager(const ComponentManager&) = delete;

		template <typename T>
		std::vector<T*> all_of(const char* name);

		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		T* get(const char* name);

		template <typename T>
		bool remove(T* ptr);

	public:
		void update();

	public:
		static ComponentManager* get_singleton_ptr();

	private:
		std::vector<Component*> m_instances;

	};

	class XPLICIT_API Component 
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component& operator=(const Component&) = default;
		Component(const Component&) = default;

	public:
		enum INSTANCE_TYPE : uint8_t 
		{
			INSTANCE_ACTOR, // Engine Actor (According to which side you're on (client or server))
			INSTANCE_PLAYER,
			INSTANCE_LOGIC, // Generic Component
			INSTANCE_CAMERA, // Camera instance
			INSTANCE_SCRIPT, // Script instance
			INSTANCE_RENDER, // An instance which can be rendered
			INSTANCE_NETWORK, // Network instance
			INSTANCE_PHYSICS, // Physics instance
			INSTANCE_GUI, // GUI instance
			INSTANCE_COUNT // the number of Instances type we have here!
		};

		enum PHYSICS_TYPE : uint8_t
		{
			PHYSICS_SIMPLE, /* Rigid-body */
			PHYSICS_COMPLEX, /* Complex Physics, Soft-body */
			PHYSICS_NONE,
			PHYSICS_COUNT,
		};

	public:
		virtual bool should_update() noexcept;
		virtual void update();

	public:
		virtual const char* name() noexcept;
		virtual INSTANCE_TYPE type() noexcept;

	public:
		virtual bool can_collide() noexcept;
		virtual bool has_physics() noexcept;
		virtual PHYSICS_TYPE physics() noexcept;

	};
}

#include "Component.inl"