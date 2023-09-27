/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "MeshComponent.h"
#include "GameVar.h"

namespace XPX
{
	MeshComponent::MeshComponent(const char* path, const std::size_t id)
		:
		mPath(path), mNode(nullptr),
		mPhysics(PHYSICS_NONE),
		mMdl(nullptr)
	{
		XPLICIT_GET_DATA_DIR(XPLICIT_DIR);

		String _path = XPLICIT_DIR;
		_path += path;

		mMdl = RENDERER->getSceneManager()->getMesh(_path.c_str());

		mPhysics = PHYSICS_NONE;

		if (mMdl)
		{
			mNode = RENDERER->getSceneManager()->addAnimatedMeshSceneNode(mMdl);
			
			// for the physics engine.
			mPhysics = PHYSICS_COMPLEX;

			mNode->setID(id);
			mNode->setPosition(vector3df(0, 0, 0));

			mMdl->setMaterialFlag(video::EMF_LIGHTING, false);
		}
		else
		{
			XPLICIT_CRITICAL("Model not found!");
			XPLICIT_CRITICAL(path);
		}
	}

	MeshComponent::~MeshComponent() noexcept
	{
		if (mNode)
			mNode->drop();

		if (mMdl)
			mMdl->drop();
	}

	bool MeshComponent::should_update() { return false; }

	void MeshComponent::update(ClassPtr class_ptr)
	{
		
	}

	const String& MeshComponent::path() noexcept { return mPath; }

	bool MeshComponent::has_physics() noexcept { return mPhysics == PHYSICS_COMPLEX; }
}