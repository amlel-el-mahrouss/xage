/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Instance.h
 *			Purpose: Xplicit's ECS
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit 
{
	class InstanceManager;
	class Instance;

	class XPLICIT_API InstanceManager final 
	{
	private:
		InstanceManager() = default;

	public:
		~InstanceManager() = default;

		InstanceManager& operator=(const InstanceManager&) = delete;
		InstanceManager(const InstanceManager&) = delete;

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
		static InstanceManager* get_singleton_ptr();

	private:
		std::vector<Instance*> m_instances;

	};

	class XPLICIT_API Instance 
	{
	public:
		Instance() = default;
		virtual ~Instance() = default;

		Instance& operator=(const Instance&) = default;
		Instance(const Instance&) = default;

	public:
		enum INSTANCE_TYPE : uint8_t 
		{
			INSTANCE_ACTOR, // Engine Actor
			INSTANCE_LOGIC, // Generic Logic Instance
			INSTANCE_CAMERA, // Camera instance
			INSTANCE_SCRIPT, // C#/Lua instance
			INSTANCE_RENDER, // Renderable instance.
			INSTANCE_NETWORK, // Network instance
			INSTANCE_PHYSICS, // Physics instance
			INSTANCE_GUI, // GUI instance
			INSTANCE_COUNT
		};

		enum PHYSICS_TYPE : uint8_t
		{
			PHYSICS_SIMPLE, /* Rigidbody */
			PHYSICS_COMPLEX, /* Complex Physics, Softbody */
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

#include "Instance.inl"