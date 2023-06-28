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
#include <Nplicit.h>

namespace Xplicit::Player
{
	/* this classes gives an component a model. */
	class StaticMesh
	{
	public:
		StaticMesh() = delete;
		
		explicit StaticMesh(const char* path);
		virtual ~StaticMesh() noexcept;

		StaticMesh& operator=(const StaticMesh&) = default;
		StaticMesh(const StaticMesh&) = default;
		
		IAnimatedMeshSceneNode* node() const { return mNode; }
		IAnimatedMesh* operator->() const { return mMdl; }

	protected:
		IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		IAnimatedMesh* mMdl; // Model Data pointer, generic

	};

	/* this one combines meshes to make a bundle */
	/* example: Epic Bundle */
	class StaticBundleMesh
	{
	public:
		StaticBundleMesh() = delete;

		explicit StaticBundleMesh(const char* head, const char* torso, const char* arm, const char* leg, const char* face);
		virtual ~StaticBundleMesh() noexcept;

		StaticBundleMesh& operator=(const StaticBundleMesh&) = default;
		StaticBundleMesh(const StaticBundleMesh&) = default;

		IAnimatedMeshSceneNode* node(const std::size_t& index) const { return mParts[index].first; }
		IAnimatedMesh* model(const std::size_t& index) const { return mParts[index].second; }

	protected:
		std::vector<std::pair<IAnimatedMeshSceneNode*, IAnimatedMesh*>> mParts;
		
	};

#define XPLICIT_BUNDLE_HEAD		 (0)
#define XPLICIT_BUNDLE_LEFT_ARM  (1)
#define XPLICIT_BUNDLE_TORSO     (2)
#define XPLICIT_BUNDLE_RIGHT_ARM (3)
#define XPLICIT_BUNDLE_LEFT_LEG  (4)
#define XPLICIT_BUNDLE_RIGHT_LEG (5)

#define XPLICIT_BUNDLE_MAX (6)

	class DynamicMesh : public ISceneNode
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