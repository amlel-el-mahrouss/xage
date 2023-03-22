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
	/// <summary>
	/// The Component loads according to the Studio/ directory.
	/// </summary>
	class MeshComponent
	{
	public:
		MeshComponent() = delete;

		MeshComponent(const char* path);
		virtual ~MeshComponent();

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	protected:
		IAnimatedMesh* _Model; // Model Data pointer, generic
		IAnimatedMeshSceneNode* _Node; // Model Data pointer, generic

	};
}