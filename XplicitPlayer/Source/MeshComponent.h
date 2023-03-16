/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: MeshComponent.h
 *			Purpose:
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <ApplicationContext.h>

namespace Xplicit::Client
{
	class MeshComponent final
	{
	public:
		MeshComponent(const char* path) noexcept 
			: node(nullptr), model(nullptr)
		{
			model = IRR->getSceneManager()->getMesh(path);

			if (model)
			{
				node = IRR->getSceneManager()->addAnimatedMeshSceneNode(model);
			}
		}

		~MeshComponent()
		{
			if (node)
				node->drop();

			if (model)
				model->drop();

		}

		MeshComponent& operator=(const MeshComponent&) = default;
		MeshComponent(const MeshComponent&) = default;

		IAnimatedMeshSceneNode* operator->() const 
		{
			XPLICIT_ASSERT(node);
			return node; 
		}

	public:
		IAnimatedMeshSceneNode* node;
		IAnimatedMesh* model;

	};
}