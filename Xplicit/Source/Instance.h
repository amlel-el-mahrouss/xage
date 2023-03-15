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

#include "Foundation.h"

namespace Xplicit 
{
	class InstanceManager;
	class Instance;

	class XPLICIT_API InstanceManager final 
	{
	private:
		InstanceManager() {}

	public:
		~InstanceManager() {}

		InstanceManager& operator=(const InstanceManager&) = default;
		InstanceManager(const InstanceManager&) = default;

		template <typename T>
		std::vector<T*> get_all(const char* name);

		template <typename T, typename... Args>
		T* add(Args&&... args);

		template <typename T>
		bool remove(T* ptr);

		template <typename T>
		bool remove(const char* name);

		template <typename T>
		T* get(const char* name);

	public:
		void update() noexcept;

	public:
		static InstanceManager* get_singleton_ptr();

	private:
		std::vector<Instance*> m_instances;

		friend class Instance;

	};

	class XPLICIT_API Instance 
	{
	public:
		Instance() {}
		virtual ~Instance() {}

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
			PHYSICS_FAST, // Fast physics calculation
			PHYSICS_COMPLEX, // Complex physics calculation
			PHYSICS_NONE,
			PHYSICS_COUNT,
		};

	public:
		virtual void update();
		virtual bool should_update() noexcept;

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