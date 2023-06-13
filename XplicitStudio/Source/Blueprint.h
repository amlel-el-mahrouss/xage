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
	class Blueprint final
	{
	public:
		Blueprint();
		~Blueprint();

	public:
		XPLICIT_COPY_DEFAULT(Blueprint);

	public:
		/* warning! this file uses a detached thread. assume parallels cases. */
		void detach(Blueprint* pBlueprint);
		void attach(Blueprint* pBlueprint);

	private:
		Blueprint* mPrev;
		Blueprint* mNext;

	};
}