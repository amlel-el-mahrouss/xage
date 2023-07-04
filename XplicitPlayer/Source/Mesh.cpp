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
	StaticMesh::StaticMesh(const char* path, const char* name, const char* group)
		: mPath(path), mPhysics(PHYSICS_NONE), mGroup(group), mName(name)
	{
		static XPLICIT_GET_DATA_DIR(dir);

		String _path = dir;
		_path += "/XplicitNgin/Contents/";
		_path += path;

		mMdl = RENDER->_getCurrentSceneManager()->createEntity(_path);

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mNode = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			mNode->attachObject(mMdl);

			mPhysics = PHYSICS_COMPLEX;
		}
	}

	StaticMesh::~StaticMesh() noexcept
	{
		if (mNode)
			delete mNode;

		if (mMdl)
			delete mMdl;
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

		auto ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Left";
		_path += arm;

		ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += torso;

		ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Right";
		_path += arm;

		ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Left";
		_path += leg;

		ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		_path.clear();

		_path = dir;
		_path += XPLICIT_BUNDLE_PATH;
		_path += "Right";
		_path += leg;

		ent = RENDER->_getCurrentSceneManager()->createEntity(_path);

		if (ent)
		{
			auto node = RENDER->_getCurrentSceneManager()->getRootSceneNode()->createChildSceneNode();
			node->attachObject(ent);

			mParts.push_back(std::make_pair(node, ent));
		}

		XPLICIT_INFO("Loaded StaticBundleMesh successfully!");
	}

	StaticBundleMesh::~StaticBundleMesh() noexcept
	{
		for (auto& part : mParts)
		{
			if (part.first)
				delete part.first;

			if (part.second)
				delete part.second;
		}
	}
}