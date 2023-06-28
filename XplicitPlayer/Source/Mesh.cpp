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
		_path += "/XplicitNgin/Contents/R6X/";
		_path += head;

		auto _head_ptr = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_head_ptr), _head_ptr));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/R6X/";
		_path += "Left";
		_path += arm;

		auto _left_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_left_arm), _left_arm));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/R6X/";
		_path += torso;

		auto _torso = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_torso), _torso));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/R6X/";
		_path += "Right";
		_path += arm;

		auto _right_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_right_arm), _right_arm));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/R6X/";
		_path += "Left";
		_path += leg;

		auto _leg_left = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_leg_left), _leg_left));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/R6X/";
		_path += "Right";
		_path += leg;

		auto _leg_right = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_leg_right), _leg_right));

		auto mesh_torso = mParts[XPLICIT_BUNDLE_TORSO].first;
		auto mesh_head = mParts[XPLICIT_BUNDLE_HEAD].first;

		auto left_arm = mParts[XPLICIT_BUNDLE_LEFT_ARM].first;
		auto right_arm = mParts[XPLICIT_BUNDLE_RIGHT_ARM].first;

		auto left_leg = mParts[XPLICIT_BUNDLE_LEFT_LEG].first;
		auto right_leg = mParts[XPLICIT_BUNDLE_RIGHT_LEG].first;

		mesh_torso->setJointMode(E_JOINT_UPDATE_ON_RENDER::EJUOR_CONTROL);

		mesh_torso->setPosition(vector3df(0.f, 2.f, 0.f));
		mesh_torso->setRotation(vector3df(90, 0, 0));

		_path.clear();

		_path = XPLICIT_ENV("APPDATA");
		_path += "/XplicitNgin/Contents/";
		_path += "R6Face.png";

		/* set origin of model. */

		mesh_head->getMaterial(0).setTexture(0, RENDER->getVideoDriver()->getTexture(_path.c_str()));

		mesh_torso->setPosition(vector3df(0.f, 2.f, 0.f));

		mesh_head->setParent(mesh_torso);
		mesh_head->setPosition(vector3df(0.f, 3.5f, 0.f));

		left_arm->setParent(mesh_torso);
		left_arm->setPosition(vector3df(-1.5f, 2.f, 0.f));

		right_arm->setParent(mesh_torso);
		right_arm->setPosition(vector3df(1.5f, 2.f, 0.f));

		right_leg->setParent(mesh_torso);
		right_leg->setPosition(vector3df(0.5f, 0.f, 0.f));

		left_leg->setParent(mesh_torso);
		left_leg->setPosition(vector3df(-0.5f, 0.f, 0.f));

		left_leg->setRotation(vector3df(90, 0, 0));
		left_arm->setRotation(vector3df(90, 0, 0));
		right_leg->setRotation(vector3df(90, 0, 0));
		right_arm->setRotation(vector3df(90, 0, 0));

		mesh_torso->setRotation(vector3df(90, 0, 0));
		mesh_head->setRotation(vector3df(90, 0, 0));

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