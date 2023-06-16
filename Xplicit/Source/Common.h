/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Common.h
 *			Purpose: Common Engine Macros/Components/Properties
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

#include "Component.h"

#define XPLICIT_MOVEMENT_RATE	(50U)
#define XPLICIT_DEFAULT_HEALTH	(100U)
#define XPLICIT_DELTA_TIME		(1000.f)
#define XPLICIT_COOLDOWN		(1000)

namespace Xplicit
{
	class XPLICIT_API PositionComponent final : public Component
	{
	public:
		explicit PositionComponent() : Position(0.f, 0.f, 0.f) {}
		virtual ~PositionComponent() = default;

		XPLICIT_COPY_DEFAULT(PositionComponent);

	public:
		String serialize() noexcept
		{
			String serialized;
			serialized = "<Pos X=\"" + std::to_string(Position.X) + "\" Y=\"" + std::to_string(Position.Y) + "\" Z=\"" + std::to_string(Position.Z) + "\" />";

			return serialized;
		}

	public:
		Quaternion<float> Position;

	};

	class XPLICIT_API SizeComponent final : public Component
	{
	public:
		explicit SizeComponent() : Size(0.f, 0.f, 0.f) {}
		virtual ~SizeComponent() = default;

		XPLICIT_COPY_DEFAULT(SizeComponent);

	public:
		String serialize() noexcept
		{
			String serialized;
			serialized = "<Size Width=\"" + std::to_string(Size.X) + "\" Height=\"" + std::to_string(Size.Y) + "\" />";

			return serialized;
		}

	public:
		Vector<float> Size;

	};

	class XPLICIT_API RenderableComponent : public Component
	{
	public:
		RenderableComponent() = default;
		virtual ~RenderableComponent() = default;

		XPLICIT_COPY_DEFAULT(RenderableComponent);

	public:
		PositionComponent Position;
		SizeComponent Size;

	};

	/* Base Service Class */
	/* Used that to make UIService, NetworkService, DownloadService */
	class XPLICIT_API ServiceComponent : public Component
	{
	public:
		ServiceComponent(const String& name) : mName(name) {}
		virtual ~ServiceComponent() = default;

	public:
		XPLICIT_COPY_DEFAULT(ServiceComponent);

	public:
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_SERVICE; }
		virtual const char* name() noexcept override { return mName.c_str(); }

	private:
		String mName;

	};

	enum class COMPONENT_BASE_LIST : std::int32_t
	{
		COMPONENT_SCRIPT = 100,
		COMPONENT_MODEL,
		COMPONENT_SOUND,
		COMPONENT_TEXTURE,
		COMPONENT_INVALID = 0xFFF,
	};
}