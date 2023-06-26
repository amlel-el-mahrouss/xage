/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <Xplicit.h>
#include <DataValue.h>

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
			this->setVisible(RENDER->getVideoDriver()->getOcclusionQueryResult(this) < 0);

			if (!this->isVisible())
				return;

			IVideoDriver* driver = SceneManager->getVideoDriver();

			driver->setMaterial(Material);
			driver->setTransform(ETS_WORLD, AbsoluteTransformation);
			driver->drawVertexPrimitiveList(&
				Vertices.data()[0], 
				4, 
				&Indices.data()[0], 
				4, 
				EVT_STANDARD, 
				EPT_TRIANGLES, 
				EIT_32BIT);
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
		
		DynamicMesh& operator=(const DynamicMesh&) = default;
		DynamicMesh(const DynamicMesh&) = default;
		
		std::vector<S3DVertex> Vertices;
		std::vector<u16> Indices;
		SMaterial Material;
		aabbox3d<f32> Box;

	};
}