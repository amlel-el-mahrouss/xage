/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver API, replacement of the LegacyRenderer (irrlicht)
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

#include <map>
#include <CLua/CLua.hpp>

namespace Xplicit::Renderer
{
	/// <summary>
	/// Tells what rendering api we're using.
	/// </summary>
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		OPENGL,
		VULKAN,
		INVALID,
	};

	/// <summary>
	/// A generic DriverSystem
	/// </summary>
	class XPLICIT_API DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api() noexcept;

	};

	enum XPLICIT_SHADER_FORMAT : uint8_t 
	{
		FORMAT_GLSL, // Vulkan legacy shader.
		FORMAT_HLSL, // Direct3D 11 shader type.
		FORMAT_COUNT,
	};

	/// <summary>
	/// Generic Shader System
	/// </summary>
	class XPLICIT_API ShaderSystem
	{
	public:
		ShaderSystem() = delete;

	public:
		explicit ShaderSystem(const PChar* filename, uint8_t format = FORMAT_HLSL)
			: m_shader(filename), m_type(format)
		{}

		virtual ~ShaderSystem() = default;

		ShaderSystem& operator=(const ShaderSystem&) = default;
		ShaderSystem(const ShaderSystem&) = default;

		const PString& shader() noexcept { return m_shader; }
		const uint8_t& type() noexcept { return m_type; }

	public:
		virtual int compile() = 0;

	protected:
		PString m_shader;
		uint8_t m_type;

	};

	/// <summary>
	/// RENDER_TYPE describes the type of render we're doing
	/// (Mesh, Texture, Polygon, Particles)
	/// </summary>
	enum class RENDER_TYPE
	{
		RENDER_MESH, // .dae meshes (.fbx support is planned)
		RENDER_TEXTURE, // ID3D11Texture
		RENDER_POLYGON, // applies for 2d and 3d contexts.
		RENDER_PARTICLE_SYSTEM, // Emitter
		RENDER_LIGHT,
		RENDER_TYPE_COUNT,
	};


	//! Moddable mesh by Lua, render system generic.
	//! Only selected when the New Rendering System is choosen.

	template <typename RenderSystemMesh>
	class XPLICIT_API RenderNode : public RenderSystemMesh
	{
	public:
		RenderNode() = default;
		~RenderNode() override = default;

	public:
		XPLICIT_COPY_DELETE(RenderNode);

	public:
		void update() override
		{
			RenderSystemMesh::update();

			if (Parent)
			{
				if (IndexState == INDEX_STATE::STOP)
				{
					IndexState = INDEX_STATE::RUNNING;

					Thread([&]() {
						Visible = Parent->index_as_bool("Visible");
						RenderDistance = Parent->index_as_bool("RenderDistance");

						Scale.X = Parent->index_as_number<float>("Scale.X");
						Scale.Y = Parent->index_as_number<float>("Scale.Y");
						Scale.Z = Parent->index_as_number<float>("Scale.Z");

						Position.X = Parent->index_as_number<float>("Position.X");
						Position.Y = Parent->index_as_number<float>("Position.Y");
						Position.Z = Parent->index_as_number<float>("Position.Z");
						});
				}

			}
		}

	public:
		enum class INDEX_STATE
		{
			STOP,
			RUNNING
		};

		static constexpr int MAX_STATE = 2;

	public:
		Lua::CLuaClass* Parent; //! Parent Lua context.
		INDEX_STATE IndexState;

	public:
		Vector<float> Position;
		float RenderDistance;
		Vector<float> Scale;
		bool Visible;

	};
}

#define XPLICIT_DRIVER_SYSTEM_OVERRIDE()\
		virtual const char* name() noexcept override;\
		virtual RENDER_SYSTEM api() noexcept override;