/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: DriverSystem.h
 *			Purpose: C++ Rendering Driver API, replacement of the LegacyRenderer (irrlicht)
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "Component.h"

#ifndef __NPLICIT_DLL__

#	include <NplicitNgine.h>

#endif // ifndef __NPLICIT_DLL__

#include <map>

namespace XPX::Renderer
{
	/// <summary>
	/// Image descrptor for Targa, Png, DDS...
	/// </summary>
	struct XPLICIT_API ImageDataParams final
	{
		void* pImage{ nullptr };

		UINT iWidth{ 800 };
		UINT iHeight{ 600 };
		UINT iStride{ 4 };

		static ImageDataParams invald_image_data()
		{
			return ImageDataParams();
		}

	};

	/// <summary>
	/// Basic renderable component, it's up to you to tell how to render it.
	/// </summary>
	class XPLICIT_API BaseRenderableComponent : public Component
	{
	public:
		BaseRenderableComponent() noexcept {}
		virtual ~BaseRenderableComponent() {}

	public:
		XPLICIT_COPY_DEFAULT(BaseRenderableComponent);

	public:
		std::size_t f_iSceneId{ 0 };

	protected:
		std::vector<Color<float>> m_arrayColorsSpecular;
		std::vector<Color<float>> m_arrayColorsDiffuse;
		std::vector<Color<float>> m_arrayColorsAmbient;
		std::vector<Vector<float>> m_arrayNormal;
		std::vector<Vector<float>> m_arrayVerts;
		std::vector<UINT> m_arrayIndices;

	};

	/// <summary>
	/// Light system: can represent the sun, lamp or whatever
	/// </summary>
	class XPLICIT_API LightSystem
	{
	public:
		explicit LightSystem() = default;
		virtual ~LightSystem() = default;

	public:
		XPLICIT_COPY_DEFAULT(LightSystem);

	public:
		Vector<float> f_vDirection{ 0, 0, 0 };
		Vector<float> f_vPosition{ 1, 1, 1 };
		Color<float> f_cAmbient{ 1, 1, 1, 1 };
		Color<float> f_cSpecular{ 1, 1, 1, 1 };
		float32 f_fPower{ 1 };

	};

	/// <summary>
	/// Tells what rendering API we're using.
	/// </summary>
	enum class RENDER_SYSTEM : uint8_t
	{
		DIRECT3D11,
		OPENGL,
		VULKAN,
		INVALID,
	};

	/// <summary>
	/// Generic driver class for any rendering cases.
	/// </summary>
	
	class XPLICIT_API DriverSystem
	{
	public:
		DriverSystem() {}
		virtual ~DriverSystem() {}

	public:
		DriverSystem& operator=(const DriverSystem&) = default;
		DriverSystem(const DriverSystem&) = default;

	public:
		virtual const char* name() noexcept;
		virtual RENDER_SYSTEM api() noexcept;

	};

	enum XPLICIT_SHADER_FORMAT : uint8_t 
	{
		FORMAT_SPIRV,
		FORMAT_GLSL, // Vulkan/OGL
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
		explicit ShaderSystem(const PChar* filename, uint8_t type, uint8_t format = FORMAT_HLSL)
			: m_shader(filename), m_format(format), m_type(type)
		{}

		virtual ~ShaderSystem() = default;

		ShaderSystem& operator=(const ShaderSystem&) = default;
		ShaderSystem(const ShaderSystem&) = default;

		const PString& shader() noexcept { return m_shader; }
		const uint8_t& format() noexcept { return m_format; }
		const uint8_t& type() noexcept { return m_type; }

	public:
		virtual int compile() = 0;

	protected:
		uint8_t m_format;
		PString m_shader;
		uint8_t m_type;

	};

	/// <summary>
	/// RENDER_TYPE describes the type of render we're doing
	/// (Mesh, Texture, Polygon, Particles)
	/// </summary>

	enum class RENDER_TYPE
	{
		RENDER_MESH, // fbx
		RENDER_TEXTURE, // targa
		RENDER_POLYGON, // raw meshes
		RENDER_PARTICLE_SYSTEM, // Fire, blood...
		RENDER_LIGHT,
		RENDER_TYPE_COUNT,
	};

	class XPLICIT_API DriverCameraSystem
	{
	public:
		explicit DriverCameraSystem() noexcept : m_vPos(0, 0, 0), m_vRot(0, 0, 0) {}
		virtual ~DriverCameraSystem() {}

	public:
		XPLICIT_COPY_DEFAULT(DriverCameraSystem);

	public:
		void set_position(const Vector<float>& pos) { m_vPos = pos; }
		void set_rotation(const Vector<float>& rot) { m_vRot = rot; }

	public:
		Vector<float>& position() noexcept { return m_vPos; }
		Vector<float>& rotation() noexcept { return m_vRot; }

	protected:
		Vector<float> m_vPos;
		Vector<float> m_vRot;

	};
}

#define XPLICIT_DRIVER_SYSTEM_OVERRIDE()\
		virtual const char* name() noexcept override;\
		virtual RENDER_SYSTEM api() noexcept override;
