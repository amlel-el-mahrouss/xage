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

#include <lua/lua.hpp>

namespace Xplicit
{
	class XPLICIT_API InstanceComponent : public Component
	{
	public:
		InstanceComponent() = delete;

	public:
		explicit InstanceComponent(
			const Vector<float>& position, 
			const Vector<float>& size,
			const Color<float>& color,
			const char* script = nullptr,
			const char* parent = "World",
			const char* name = "Instance",
			const bool read_only = false);

		~InstanceComponent() override;

	public:
		InstanceComponent& operator=(const InstanceComponent&) = default;
		InstanceComponent(const InstanceComponent&) = default;

	public:
		COMPONENT_TYPE type() noexcept override { return COMPONENT_GENERIC; }

		const char* name() noexcept override { return mName.c_str(); }
		const char* parent() noexcept;

	public:
		bool should_update() noexcept override;
		void update() override;

	public:
		XAttribute& get_attribute() noexcept;

	private:
		XAttribute mAttribute;
		String mParent;
		bool mReadOnly;
		String mName;

	};
}