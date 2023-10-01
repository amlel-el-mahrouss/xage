/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

 //! This file handles the engine component system stuff.

namespace XPX
{
	enum COMPONENT_TYPE : uint8_t
	{
		COMPONENT_HUMANOID, // Humanoid (Player and NPC)
		COMPONENT_LOGIC, // Logic Component
		COMPONENT_CAMERA, // Camera component
		COMPONENT_RENDER, // Script component (C#, Lua)
		COMPONENT_WATER, // Water component (perlin noise)
		COMPONENT_NETWORK, // Network component
		COMPONENT_PHYSICS, // Physics component
		COMPONENT_GUI, // GUI component
		COMPONENT_PEDESTRIAN, // Something that is alive.
		COMPONENT_VEHICLE, // ranges from sedans to motors
		COMPONENT_REPLICATION, // Replicated component
		COMPONENT_SOUND, // Sound component
		COMPONENT_GENERIC,
		COMPONENT_SCRIPT,
		COMPONENT_COUNT // the number of components type we have registered here!
	};

	enum PHYSICS_TYPE : uint8_t
	{
		PHYSICS_SIMPLE, /* Rigid-body */
		PHYSICS_COMPLEX, /* Soft-body */
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

			~ComponentAccessor() = default;

		public:
			ComponentUpdateEvalAccessor _Eval;
			ComponentUpdateAccessor _Update;
			ComponentData _Pointee;
			String _AbiName;
			String _Name;

		public:
			const String abi_name() const noexcept { return _AbiName; }
			const String name() const noexcept { return _Name; }

		public:
			template <typename Y>
			Y as_type() const noexcept
			{
				return reinterpret_cast<Y>(_Pointee);
			}
		};
	}

	class XPLICIT_API ComponentSystem final
	{
	public:
		explicit ComponentSystem() = default;
		~ComponentSystem() = default;

	public:
		XPLICIT_COPY_DELETE(ComponentSystem);

	public:
		template <typename T, typename... Args>
		T* add(Args&&... args);

	public:
		template <typename T>
		bool erase(std::vector<Details::ComponentAccessor>::const_iterator& filter);

		template <typename T>
		bool remove(T* ptr);

	public:
		std::vector<Details::ComponentAccessor>& get_all() noexcept;

	public:
		template <typename T>
		std::vector<T*> all_of();

		template <typename T>
		T* get(const char* filter);

	public:
		void update();

	public:
		static ComponentSystem* get_singleton_ptr() noexcept;

	private:
		std::vector<Details::ComponentAccessor> mComponents;

	};
}

#define XPLICIT_COMPONENT_OVERRIDE()\
		const char* name() noexcept;\
		COMPONENT_TYPE type() noexcept override;\
		static bool should_update() noexcept;\
		static void update(void* class_ptr);\
		PHYSICS_TYPE physics() noexcept override;\
		bool has_physics() noexcept override;\
		bool can_collide() noexcept override;


#include "Component.inl"
