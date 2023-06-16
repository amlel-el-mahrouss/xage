/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: MeshComponent.cpp
 *			Purpose: Custom Mesh Component (.dae or .xm)
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "IMeshable.h"

namespace Xplicit::Player
{
	IMeshable::IMeshable(const char* path)
	{
		String _path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Studio/";
		_path += path;

		mMdl = IRR->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(mMdl);

		mNode = IRR->getSceneManager()->addAnimatedMeshSceneNode(mMdl);
		XPLICIT_ASSERT(mNode);
	
		if (mMdl)
		{
			mMdl->setMaterialFlag(EMF_LIGHTING, false);
			mMdl->setAnimationSpeed(240.f);
		}
	}

	IMeshable::~IMeshable()
	{
		if (mNode)
			mNode->drop();
	}

}