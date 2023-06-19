/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Application.h"
#include <Common.h>
#include "Mesh.h"

namespace Xplicit::Player
{
	class LocalToolInventory final
	{
	public:
		LocalToolInventory() = default;
		~LocalToolInventory() = default;

		static constexpr const std::size_t max = 72;

	public:
		XPLICIT_COPY_DEFAULT(LocalToolInventory);

	public:
		ToolComponent* create(const char* name)
		{
			ToolComponent* tool = mPool.allocate();

			if (tool == nullptr)
				throw EngineError("Inventory is full!");

			tool->Name = name;
			tool->Owner = -1;
			tool->Slot = -1;
			tool->Droppable = false;

			Tools[mPool.nalloc() - 1] = tool;

			return tool;
		}

		void destroy(ToolComponent* component) 
		{
			Thread job([&]() {
				try
				{
					const std::size_t sz = mPool.size();

					for (std::size_t toolIndex = 0; toolIndex < sz; ++toolIndex)
					{
						if (Tools[toolIndex] == component)
						{
							Tools[toolIndex] == nullptr;

							if (component)
								mPool.deallocate(component);

							return;
						}
					}
				}
				catch (...)
				{
					std::exit(3);
				}
			});

			job.detach();
		}

	public:
		ToolComponent* Tools[LocalToolInventory::max];

	private:
		Pool<ToolComponent, LocalToolInventory::max> mPool;

	};
}