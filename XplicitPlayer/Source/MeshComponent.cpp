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
		String _path = XPLICIT_ENV("APPDATA");
		_path += "/Data/Studio/";
		_path += path;

		m_pModel = IRR->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(m_pModel);

		m_pNode = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_pModel);
		XPLICIT_ASSERT(m_pNode);
	
		if (m_pModel)
			m_pModel->setMaterialFlag(EMF_LIGHTING, false);
	}

	MeshComponent::~MeshComponent()
	{
		if (m_pNode)
			m_pNode->drop();
	}

}