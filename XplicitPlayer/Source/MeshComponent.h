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
	class MeshComponentHelper
	{
	public:
		MeshComponentHelper() = delete;

		MeshComponentHelper(const char* path);
		virtual ~MeshComponentHelper();

		MeshComponentHelper& operator=(const MeshComponentHelper&) = default;
		MeshComponentHelper(const MeshComponentHelper&) = default;

	protected:
		IAnimatedMesh* m_model; // Model Data pointer, generic
		IAnimatedMeshSceneNode* m_node; // Model Data pointer, generic

	};
}