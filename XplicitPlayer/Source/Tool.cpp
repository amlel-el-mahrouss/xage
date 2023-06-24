/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "Tool.h"

namespace Xplicit::Player
{
	ToolComponent* LocalToolAllocator::create(const char* name)
	{
		ToolComponent* tool = mPool.allocate();

		if (tool == nullptr) 
			return nullptr;

		tool->Name = name;
		tool->Owner = -1;
		tool->Slot = -1;
		tool->Droppable = false;
		
		return tool;
	}

	void LocalToolAllocator::destroy(ToolComponent* component)
	{
		if (component)
			mPool.deallocate(component);
	}
}