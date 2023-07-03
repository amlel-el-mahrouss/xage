/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
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
		: mPath(path), mPhysics(PHYSICS_NONE)
	{
		static XPLICIT_GET_DATA_DIR(dir);

		String _path = dir;
		_path += "/XplicitNgin/Contents/";
		_path += path;

		mMdl = RENDER->getSceneManager()->getMesh(_path.c_str());

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mMdl->setMaterialFlag(EMF_LIGHTING, false);
			mNode = RENDER->getSceneManager()->addAnimatedMeshSceneNode(mMdl);

			mPhysics = PHYSICS_COMPLEX;
		}
	}

	StaticMesh::~StaticMesh() noexcept
	{
		if (mNode)
			(void)mNode->drop();

		if (mMdl)
			(void)mMdl->drop();
	}

	const String& StaticMesh::path() noexcept
	{
		return mPath;
	}

	bool StaticMesh::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX;  }

	static auto XPLICIT_BUNDLE_PATH = "Contents/Bundles/";

	StaticBundleMesh::StaticBundleMesh(const char* head, 
		const char* torso, 
		const char* arm, 
		const char* leg,
		const char* face)
	{
		static XPLICIT_GET_DATA_DIR(dir);

		String _path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += head;

		auto _head_ptr = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_head_ptr), _head_ptr));

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Left";
		_path += arm;

		auto _left_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_left_arm), _left_arm));

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += torso;

		auto _torso = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_torso), _torso));

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Right";
		_path += arm;

		auto _right_arm = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_right_arm), _right_arm));

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Left";
		_path += leg;

		auto _leg_left = RENDER->getSceneManager()->getMesh(_path.c_str());
		mParts.push_back(std::make_pair(RENDER->getSceneManager()->addAnimatedMeshSceneNode(_leg_left), _leg_left));

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
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

		_path = dir;
		_path += "/XplicitNgin/Contents/Bundles/";
		_path += face;

		/* long pointer checks, stupid shit, but we gotta do it */
		XPLICIT_ASSERT(_leg_left &&
			_leg_right &&
			_right_arm &&
			_left_arm &&
			_torso &&
			_head_ptr);

		mesh_head->getMaterial(0).setTexture(0, RENDER->getVideoDriver()->getTexture(_path.c_str()));

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