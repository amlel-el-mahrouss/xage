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

	/* max components */
	inline constexpr std::size_t XPLICIT_MAX_COMPONENTS = 1000000;

	class XPLICIT_API ComponentManager final 
	{
	private:
		ComponentManager() = default;

	public:
		~ComponentManager() = default;

	public:
		XPLICIT_COPY_DEFAULT(ComponentManager);

	public:
		template <typename T>
		std::vector<T*> all_of(const char* name);

	public:
		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		bool remove(T* ptr);

	public:
		template <typename T>
		T* get(const char* name) noexcept;

		template <typename T>
		T* get_first() noexcept;

	public:
		void update() noexcept;

	public:
		static ComponentManager* get_singleton_ptr() noexcept;

	private:
		std::vector<Component*> mComponents;

	};

	enum COMPONENT_TYPE : uint8_t
	{
		COMPONENT_ACTOR, // Engine Actor (According to which side you're on (client or server))
		COMPONENT_PLAYER, // Player
		COMPONENT_LOGIC, // Generic Component
		COMPONENT_CAMERA, // Camera instance
		COMPONENT_SCRIPT, // Script instance
		COMPONENT_RENDER, // An instance which can be rendered
		COMPONENT_NETWORK, // Network instance
		COMPONENT_PHYSICS, // Physics instance
		COMPONENT_GUI, // GUI instance
		COMPONENT_REPLICATION, // Replicated instance
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
		Component() = default;
		virtual ~Component() = default;

	public:
		XPLICIT_COPY_DEFAULT(Component);

	public:
		virtual bool should_update() noexcept;
		virtual void update();

	public:
		virtual COMPONENT_TYPE type() noexcept;
		virtual const char* name() noexcept;

	public:
		virtual PHYSICS_TYPE physics() noexcept;
		virtual bool can_collide() noexcept;
		virtual bool has_physics() noexcept;

	};
}

#include "Component.inl"