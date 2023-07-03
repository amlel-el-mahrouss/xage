/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	class InstanceComponent : public Component
	{
	public:
		InstanceComponent() = delete;

	public:
		explicit InstanceComponent(
			const Vector<float>& position, 
			const Vector<float>& size,
			const Color<float>& color,
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