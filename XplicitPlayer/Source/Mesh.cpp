/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: MeshComponent.cpp
 *			Purpose: Custom Mesh Component (.dae or streaming meshes)
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <Root.h>
#include "Mesh.h"

namespace Xplicit::Player
{
	StaticMesh::StaticMesh(const char* path)
	{
		String _path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += path;

		mMdl = IRR->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(mMdl);

		mNode = IRR->getSceneManager()->addAnimatedMeshSceneNode(mMdl);
		XPLICIT_ASSERT(mNode);
	
		if (mMdl)
			mMdl->setMaterialFlag(EMF_LIGHTING, true);
	}
}