/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Corporation, all rights reserved.
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
			const char* parent = "Work",
			const char* name = "Part");

		~InstanceComponent();

		InstanceComponent& operator=(const InstanceComponent&) = default;
		InstanceComponent(const InstanceComponent&) = default;

		COMPONENT_TYPE type() noexcept override { return COMPONENT_GENERIC; }
		const char* name() noexcept override { return mName.c_str(); }

		bool should_update() noexcept override;
		void update() override;

		XAttribute& get_attribute() noexcept;

	private:
		XAttribute mAttribute;
		String mParent;
		String mName;

	};
}