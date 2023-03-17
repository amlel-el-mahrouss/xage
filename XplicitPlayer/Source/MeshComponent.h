/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: MeshComponent.h
 *			Purpose: Custom Mesh Component (.dae or .xm)
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <ApplicationContext.h>

namespace Xplicit::Client
{
	using MeshPrivateData = void*;

	// Render Traits
	class MeshComponent
	{
	public:
		MeshComponent() = default;
		virtual ~MeshComponent() = default;

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	public:
		MeshPrivateData _Ptr; // Model Data pointer, generic

	};
}