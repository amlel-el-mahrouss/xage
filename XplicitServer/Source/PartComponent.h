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
	class PartComponent : public Component
	{
	public:
		PartComponent() = delete;

	public:
		explicit PartComponent(const Vector<float>& position, const Vector<float>& size, const Color<float>& color, const char* name = "Part");
		virtual ~PartComponent();

		PartComponent& operator=(const PartComponent&) = default;
		PartComponent(const PartComponent&) = default;

		virtual COMPONENT_TYPE type() noexcept override { return COMPONENT_LOGIC; }
		virtual const char* name() noexcept override { return ("PartComponent"); }

		virtual bool should_update() noexcept override;
		virtual void update() override;

		XAttribute& get_attribute() noexcept;

	private:
		XAttribute mAttribute;
		String mName;

	};
}