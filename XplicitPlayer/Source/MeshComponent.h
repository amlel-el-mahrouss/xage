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
	using MeshPrivateData = IAnimatedMesh;
	using MeshPrivateData2 = IAnimatedMeshSceneNode;

	// Render Traits
	class MeshComponent
	{
	public:
		MeshComponent(const char* path)
		{
			_Model = IRR->getSceneManager()->getMesh(path);
			XPLICIT_ASSERT(_Model);

			_Node = IRR->getSceneManager()->addAnimatedMeshSceneNode(_Model);
			XPLICIT_ASSERT(_Node);
		}

		virtual ~MeshComponent()
		{
			if (_Node)
				_Node->drop();
		}

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

	protected:
		MeshPrivateData* _Model; // Model Data pointer, generic
		MeshPrivateData2* _Node; // Model Data pointer, generic

	};
}