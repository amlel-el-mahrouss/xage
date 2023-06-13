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

namespace Xplicit::Player
{
	/* this classes gives an component a model. */
	class MeshComponent
	{
	public:
		MeshComponent() = delete;

		MeshComponent(const char* path);
		virtual ~MeshComponent();

	public:
		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	protected:
		IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		IAnimatedMesh* mMdl; // Model Data pointer, generic

	};
}