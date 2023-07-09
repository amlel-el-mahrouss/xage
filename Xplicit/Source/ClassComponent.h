/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonEngine.h"
#include "Component.h"
#include "Util.h"

#include <CLua/CLua.hpp>

#define XPLICIT_CLASS_NAMESPACE "World."

namespace Xplicit
{
	class XPLICIT_API ClassComponent : public Component, public Lua::CLuaClass
	{
	public:
		ClassComponent() = delete;

	public:
		explicit ClassComponent(
			const Vector<float>& position, 
			const Vector<float>& size,
			const Color<float>& color,
			const char* script = nullptr,
			const char* parent = XPLICIT_CLASS_NAMESPACE,
			const char* name = "Class");

		~ClassComponent() override;

	public:
		ClassComponent& operator=(const ClassComponent&) = default;
		ClassComponent(const ClassComponent&) = default;

	public:
		COMPONENT_TYPE type() noexcept override { return COMPONENT_GENERIC; }

	public:
		const char* name() noexcept override { return mName.c_str(); }
		const char* parent() noexcept;

	public:
		static bool should_update() noexcept { return true; }
		static void update(void* _this);

	public:
		XAttribute& get_attribute() noexcept;

	private:
		XAttribute mAttribute;
		String mParent;
		String mName;

	};
}