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

		auto mesh_torso = mParts[XPLICIT_BUNDLE_TORSO].first;

		auto left_arm = mParts[XPLICIT_BUNDLE_LEFT_ARM].first;
		auto right_arm = mParts[XPLICIT_BUNDLE_RIGHT_ARM].first;

		auto left_leg = mParts[XPLICIT_BUNDLE_LEFT_LEG].first;
		auto right_leg = mParts[XPLICIT_BUNDLE_RIGHT_LEG].first;

		mesh_torso->setJointMode(E_JOINT_UPDATE_ON_RENDER::EJUOR_CONTROL);

		left_arm->setParent(mesh_torso);
		right_arm->setParent(mesh_torso);

		right_leg->setParent(mesh_torso);
		left_leg->setParent(mesh_torso);

		mesh_torso->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));

		left_arm->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));
		right_arm->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));

		right_leg->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));
		left_leg->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));

		XPLICIT_INFO("Loaded StaticBundleMesh successfully!");
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