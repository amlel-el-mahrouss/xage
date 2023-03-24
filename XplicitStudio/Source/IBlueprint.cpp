/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: IBlueprint.cpp
 *			Purpose: C++/C# Blueprints
 *
 * =====================================================================
 */

#include "IBlueprint.h"

namespace Xplicit::Studio
{
	IBlueprint::IBlueprint() : m_pPrev(nullptr), m_pNext(nullptr) {}

	IBlueprint::~IBlueprint()
	{
		if (m_pNext)
			m_pNext->detach(this);

		if (m_pPrev)
			m_pPrev->detach(this);
	}
}