/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Util.h"
#include "Component.h"

#include <CLua/CLua.hpp>

#define XPLICIT_CLASS_NAMESPACE "World"
#define XPLICIT_CLASS_NAME "Class"

namespace XPX
{
	class XPLICIT_API ClassComponent : public Component, public Lua::CLuaClass, public XPXAttribute
	{
	public:
		ClassComponent() = delete;

	public:
		ClassComponent(
			const Vector<float>& position, 
			const Vector<float>& size,
			const Color<float>& color,
			const char* script = nullptr,
			const char* parent = XPLICIT_CLASS_NAMESPACE,
			const char* name = XPLICIT_CLASS_NAME);

		ClassComponent(
			const char* parent,
			const char* name);

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
		static bool should_update() noexcept;
		static void update(ClassPtr self);

	private:
		String mParent;
		String mName;

	};
}
