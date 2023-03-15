/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver
 *
 * =====================================================================
 */

#pragma once

#include <Foundation.h>

namespace Xplicit::Renderer
{
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		INVALID,
		VULKAN,
	};

	class DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api();

	};
}