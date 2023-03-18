/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: MeshComponent.cpp
 *			Purpose: Custom Mesh Component (.dae or .xm)
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "MeshComponent.h"

namespace Xplicit::Client
{
	MeshComponent::MeshComponent(const char* path)
	{
		_Model = IRR->getSceneManager()->getMesh(path);
		XPLICIT_ASSERT(_Model);

		_Node = IRR->getSceneManager()->addAnimatedMeshSceneNode(_Model);
		XPLICIT_ASSERT(_Node);
	}

	MeshComponent::~MeshComponent()
	{
		if (_Node)
			_Node->drop();
	}

}