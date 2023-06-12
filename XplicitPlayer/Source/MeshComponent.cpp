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
	MeshComponentHelper::MeshComponentHelper(const char* path)
	{
		String _path = XPLICIT_ENV("APPDATA");
		_path += "/Data/Studio/";
		_path += path;

		m_model = IRR->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(m_model);

		mNode = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_model);
		XPLICIT_ASSERT(mNode);
	
		if (m_model)
			m_model->setMaterialFlag(EMF_LIGHTING, false);
	}

	MeshComponentHelper::~MeshComponentHelper()
	{
		if (mNode)
			mNode->drop();
	}

}