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
		std::string _path = XPLICIT_ENV("APPDATA");
		_path += "/Data/Studio/";
		_path += path;

		_Model = IRR->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(_Model);

		_Node = IRR->getSceneManager()->addAnimatedMeshSceneNode(_Model);
		XPLICIT_ASSERT(_Node);
	
		if (_Model)
			_Model->setMaterialFlag(EMF_LIGHTING, false);
	}

	MeshComponent::~MeshComponent()
	{
		if (_Node)
			_Node->drop();
	}

}