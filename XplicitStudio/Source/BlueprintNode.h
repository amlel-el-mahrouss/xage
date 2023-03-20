/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: BlueprintNode.h
 *			Purpose: C++/C# Blueprints Node.
 *
 * =====================================================================
 */


#pragma once

#include <Xplicit.h>
#include <CanvasComponent.h>

namespace Xplicit
{
	class BlueprintNode
	{
	public:
		BlueprintNode() = default;
		~BlueprintNode() = default;

		XPLICIT_COPY_DEFAULT(BlueprintNode);

		virtual void draw() = 0;
		virtual void update(void*) = 0;
		virtual void move(float x, float y) = 0;

	protected:
		std::vector<BlueprintNode*> m_pAttachedNodes;

	};
}