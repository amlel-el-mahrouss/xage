/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <CommonEngine.h>

#include "Application.h"
#include "Mesh.h"

namespace Xplicit::Player
{
	//!
	//! This class help create tools for players/npcs.
	//! It returns 'ToolComponent' when pool is successfully allocating bytes.
	//!

	class LocalToolAllocator final
	{
	public:
		LocalToolAllocator() = default;
		~LocalToolAllocator() = default;
		
		static constexpr const std::size_t max = 20;
		
		XPLICIT_COPY_DEFAULT(LocalToolAllocator);
		
		ToolComponent* create(const char* name);
		void destroy(ToolComponent* component);

	public:
		ToolComponent* Collection[LocalToolAllocator::max];

	private:
		Pool<ToolComponent, LocalToolAllocator::max> mPool;

	};
}