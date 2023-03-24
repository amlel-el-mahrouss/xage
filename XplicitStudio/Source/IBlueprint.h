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

namespace Xplicit::Studio
{
	class IBlueprint
	{
	public:
		IBlueprint();
		~IBlueprint();

		XPLICIT_COPY_DEFAULT(IBlueprint);

		virtual void draw() = 0;
		virtual void update(void*) = 0;
		virtual void move(float x, float y) = 0;

		virtual void detach(IBlueprint* bp) = 0;
		virtual void attach(IBlueprint* bp) = 0;

	protected:
		IBlueprint* m_pPrev;
		IBlueprint* m_pNext;

		std::vector<IBlueprint*> m_pAttachedNodes;

	};
}