/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Blueprint.h
 *			Purpose: C++ Blueprints
 *
 * =====================================================================
 */

#pragma once

#include <Bites.h>
#include <Xplicit.h>
#include <DriverD3D11.h>

namespace Xplicit::Studio
{
	class BlueprintNode final
	{
	public:
		BlueprintNode();
		~BlueprintNode();

		XPLICIT_COPY_DEFAULT(BlueprintNode);

		void detach(BlueprintNode* pBlueprint);
		void attach(BlueprintNode* pBlueprint);

	private:
		BlueprintNode* m_pPrev;
		BlueprintNode* m_pNext;

	};
}