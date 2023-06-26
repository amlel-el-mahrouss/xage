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

		mMdl = RENDER->getSceneManager()->getMesh(_path.c_str());
		XPLICIT_ASSERT(mMdl);

		mNode = RENDER->getSceneManager()->addAnimatedMeshSceneNode(mMdl);
		XPLICIT_ASSERT(mNode);
	
		if (mMdl)
			mMdl->setMaterialFlag(EMF_LIGHTING, false);
	}

	StaticMesh::~StaticMesh() noexcept
	{
		if (mNode)
			(void)mNode->drop();

		if (mMdl)
			(void)mMdl->drop();
	}

	StaticBundleMesh::StaticBundleMesh(const char* head, 
		const char* torso, 
		const char* arm, 
		const char* leg)
	{
		String _path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += head;

		auto _head_ptr = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_head_ptr), _head_ptr));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += arm;

		auto _left_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_left_arm), _left_arm));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += torso;

		auto _torso = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_torso), _torso));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += arm;

		auto _right_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_right_arm), _right_arm));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += leg;

		auto _leg_left = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_leg_left), _leg_left));

		auto _leg_right = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_leg_right), _leg_right));
	}

	StaticBundleMesh::~StaticBundleMesh() noexcept
	{
		for (auto& part : mParts)
		{
			if (part.first)
				part.first->drop();

			if (part.second)
				part.second->drop();
		}
	}
}