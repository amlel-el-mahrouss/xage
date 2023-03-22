/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: IBlueprint.h
 *			Purpose: C++/C# Blueprints
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <CanvasComponent.h>

namespace Xplicit::Scripting
{
	class IBlueprint
	{
	public:
		IBlueprint() = default;
		~IBlueprint() = default;

		XPLICIT_COPY_DEFAULT(IBlueprint);

		virtual void draw() = 0;
		virtual void update(void*) = 0;
		virtual void move(float x, float y) = 0;

	protected:
		std::vector<IBlueprint*> m_pAttachedNodes;

	};
}