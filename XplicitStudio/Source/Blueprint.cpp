/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Blueprint.cpp
 *			Purpose: C++ Blueprints
 *
 * =====================================================================
 */

#include "Blueprint.h"

namespace Xplicit::Studio
{
	BlueprintNode::BlueprintNode() 
		: m_pPrev(nullptr), m_pNext(nullptr) 
	{
	
	}

	BlueprintNode::~BlueprintNode()
	{
		if (m_pNext)
			m_pNext->detach(this);

		if (m_pPrev)
			m_pPrev->detach(this);
	}

	void BlueprintNode::detach(BlueprintNode* pBlueprint)
	{
		if (!pBlueprint)
			return;

		BlueprintNode* swap_ptr = m_pNext;

		bool found = false;

		Thread thrd = Thread([&]() -> void {
			while (!found)
			{
				if (swap_ptr)
				{
					if (swap_ptr == pBlueprint)
					{
						swap_ptr->m_pNext->m_pPrev = swap_ptr->m_pPrev;
						swap_ptr->m_pPrev->m_pNext = swap_ptr->m_pNext;

						found = true;
					}

					swap_ptr = swap_ptr->m_pPrev;

					continue;
				}

				break;
			}
		});

		thrd.detach();
	}

	void BlueprintNode::attach(BlueprintNode* pBlueprint)
	{
		auto* prev = m_pNext;
		
		m_pNext = pBlueprint;

		m_pNext->m_pPrev = prev;
		m_pNext->m_pNext = nullptr;
	}
}