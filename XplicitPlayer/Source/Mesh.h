/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>

namespace Xplicit::Player
{
	/* this classes gives an component a model. */
	class StaticMesh
	{
	public:
		StaticMesh() = delete;
		
		explicit StaticMesh(const char* path);

		~StaticMesh() noexcept
		{
			if (mNode)
				(void)mNode->drop();

			if (mMdl)
				(void)mMdl->drop();
		}
		
		StaticMesh& operator=(const StaticMesh&) = default;
		StaticMesh(const StaticMesh&) = default;
		
		IAnimatedMeshSceneNode* node() const { return mNode; }
		IAnimatedMesh* operator->() const { return mMdl; }

	protected:
		IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		IAnimatedMesh* mMdl; // Model Data pointer, generic

	};

	class DynamicMesh final : public ISceneNode
	{
	public:
		DynamicMesh(ISceneNode* parent, ISceneManager* mgr, s32 id)
			: ISceneNode(parent, mgr, id)
		{}

		~DynamicMesh() override = default;
		
		void OnRegisterSceneNode() override
		{
			if (IsVisible)
				SceneManager->registerNodeForRendering(this);

			ISceneNode::OnRegisterSceneNode();
		}

		void render() override
		{
			IVideoDriver* driver = SceneManager->getVideoDriver();

			driver->setMaterial(Material);
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
			driver->drawVertexPrimitiveList(&
				Vertices.data()[0], 
				4, 
				&Indices.data()[0], 
				4, 
				video::EVT_STANDARD, 
				scene::EPT_TRIANGLES, 
				video::EIT_32BIT);
		}
		
		void add(const u16& indice) noexcept
		{
			Indices.push_back(indice);
		}

		void add(const video::S3DVertex& vert) noexcept
		{
			Vertices.push_back(vert);

			Box.reset(Vertices[0].Pos);

			for (s32 i = 1; i < Vertices.size(); ++i)
				Box.addInternalPoint(Vertices[i].Pos);
		}

		enum
		{
			TOGGLE_WIREFRAME,
			TOGGLE_LIGHTING,
			TOGGLE_SHINY,
		};

		void toggle(const std::int32_t& toggleWhat) noexcept
		{
			switch (toggleWhat)
			{
			case TOGGLE_LIGHTING:
				this->Material.Lighting = !this->Material.Lighting;
			case TOGGLE_WIREFRAME:
				this->Material.Wireframe = !this->Material.Wireframe;
			case TOGGLE_SHINY:
				this->Material.Shininess = !this->Material.Shininess;
			default:
				break;
			}

		}

	public:
		DynamicMesh& operator=(const DynamicMesh&) = default;
		DynamicMesh(const DynamicMesh&) = default;

	public:
		std::vector<video::S3DVertex> Vertices;
		std::vector<u16> Indices;
		video::SMaterial Material;
		core::aabbox3d<f32> Box;

	};
}