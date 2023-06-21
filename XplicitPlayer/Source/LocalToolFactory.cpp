/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalToolFactory.h"

namespace Xplicit::Player
{
	ToolComponent* LocalToolFactory::create(const char* name)
	{
		ToolComponent* tool = mPool.allocate();

		if (tool == nullptr)
			throw EngineError("Inventory is full!");

		tool->Name = name;
		tool->Owner = -1;
		tool->Slot = -1;
		tool->Droppable = false;

		Tools[mPool.capacity() - 1] = tool;

		return tool;
	}

	void LocalToolFactory::destroy(ToolComponent* component)
	{
		try
		{
			const std::size_t sz = mPool.size();

			for (std::size_t toolIndex = 0; toolIndex < sz; ++toolIndex)
			{
				if (Tools[toolIndex] == component)
				{
					Tools[toolIndex] = nullptr;

					if (component)
						mPool.deallocate(component);

					break;
				}
			}
		}
		catch (...)
		{
#ifdef XPLICIT_DEBUG
			XPLICIT_INFO("Could not delloacate pointer, tool probably doesn't exist in pool!");
#endif
		}
	}
}