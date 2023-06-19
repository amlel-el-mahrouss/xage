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
#include <CommonEngine.h>
#include "Mesh.h"

namespace Xplicit::Player
{
	//!
	//! This class helps create tools for a specific context (player)
	//! returns 'ToolComponent' when pool is successfully allocating bytes.
	//! 
	class LocalToolFactory final
	{
	public:
		LocalToolFactory() = default;
		~LocalToolFactory() = default;

		static constexpr const std::size_t max = 72;

	public:
		XPLICIT_COPY_DEFAULT(LocalToolFactory);

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
#ifdef XPLICIT_DEBUG
					if (component)
						XPLICIT_INFO("Failed to deallocate: " + component->Name);
#endif // ifdef XPLICIT_DEBUG
				}
			});

			job.detach();
		}

	public:
		ToolComponent* Tools[LocalToolFactory::max];

	private:
		Pool<ToolComponent, LocalToolFactory::max> mPool;

	};
}