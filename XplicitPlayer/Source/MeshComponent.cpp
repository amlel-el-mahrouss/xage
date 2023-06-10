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

		m_node = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_model);
		XPLICIT_ASSERT(m_node);
	
		if (m_model)
			m_model->setMaterialFlag(EMF_LIGHTING, false);
	}

	MeshComponentHelper::~MeshComponentHelper()
	{
		if (m_node)
			m_node->drop();
	}

}