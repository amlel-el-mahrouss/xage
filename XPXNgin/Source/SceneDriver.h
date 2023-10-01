/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SceneDriver.h
 *			Purpose: Generic Video driver.
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
		class SceneDriver;
		class SceneLoader;

		typedef std::size_t SceneID;

		class XPLICIT_API SceneDriver final
		{
		public:
			explicit SceneDriver() noexcept;
			~SceneDriver();

		public:
			SceneDriver& operator=(const SceneDriver&) = default;
			SceneDriver(const SceneDriver&) = default;

		public:
			const SceneID add_texture(const char* path);
			const SceneID add_mesh(const char* path);

		public:
			RenderableComponent* get_scene_node(const SceneID& /* scene id */);

		public:
			void start_frame() noexcept;
			bool end_frame() noexcept;

		private:
			std::unique_ptr<ComponentSystem> m_system;
			VideoDriverSystem* m_driver;

		public:
			SceneLoader* f_textureLoader;
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
			virtual void* load_from_filesystem(const char* path) = 0;

		protected:
			FilesystemWrapper m_wrapper;

		};
	}
}
