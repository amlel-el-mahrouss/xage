/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SceneSystem.h
 *			Purpose: Rendering scene system.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#define XPX_INVALID_SCENE_ID UINT64_MAX

#ifdef XPLICIT_WINDOWS

#include "DriverD3D11.h"

namespace XPX
{
	typedef Renderer::DX11::DriverSystemD3D11 VideoDriverSystem;
}

#endif

namespace XPX
{
	namespace Renderer
	{
		class SceneSystem;
		class SceneLoader;

		typedef std::size_t SceneID;

		class XPLICIT_API SceneSystem final
		{
		public:
			explicit SceneSystem(const char* pName = "root") noexcept;
			~SceneSystem();

		public:
			SceneSystem& operator=(const SceneSystem&) = default;
			SceneSystem(const SceneSystem&) = default;

		public:
			const SceneID add_scene_node(const char* path);
			bool remove_scene_node(const SceneID& sceneId);

		public:
			RenderableComponent* get_scene_node(const SceneID& /* scene id */);

		public:
			void start_frame() noexcept;
			bool end_frame() noexcept;

		private:
			std::unique_ptr<ComponentSystem> m_system;
			VideoDriverSystem* m_driver;
			String m_name;

		public:
			SceneLoader* f_meshLoader;

		};

		class XPLICIT_API SceneLoader
		{
		public:
			explicit SceneLoader() = default;
			virtual ~SceneLoader() = default;

		public:
			XPLICIT_COPY_DEFAULT(SceneLoader);

		public:
			struct SceneData
			{
				SceneSystem* f_Parent;
				std::vector<RenderableComponent*> f_Nodes;
			};

		public:
			virtual std::shared_ptr<SceneData> from_disk(const char* path, SceneSystem* scene) = 0;

		protected:
			FilesystemWrapper m_wrapper;

		};

		class XPLICIT_API SceneLoaderXSD final : public SceneLoader
		{
		public:
			explicit SceneLoaderXSD() = default;
			~SceneLoaderXSD() override = default;

		public:
			XPLICIT_COPY_DEFAULT(SceneLoaderXSD);

		public:
			virtual std::shared_ptr<SceneData> from_disk(const char* path, SceneSystem* scene) override;

		};
	}
}
