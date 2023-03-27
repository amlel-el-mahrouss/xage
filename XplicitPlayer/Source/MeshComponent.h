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

namespace Xplicit::Client
{
	class MeshComponent
	{
	public:
		MeshComponent() = delete;

		MeshComponent(const char* path);
		virtual ~MeshComponent();

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	protected:
		IAnimatedMesh* m_pModel; // Model Data pointer, generic
		IAnimatedMeshSceneNode* m_pNode; // Model Data pointer, generic

	};
}