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
	//! This class helps create tools for a specific context (player)
	//! returns 'ToolComponent' when pool is successfully allocating bytes.
	//! 
	class LocalToolFactory final
	{
	public:
		LocalToolFactory() = default;
		~LocalToolFactory() = default;

	public:
		static constexpr const std::size_t max = 72;

	public:
		XPLICIT_COPY_DEFAULT(LocalToolFactory);

	public:
		ToolComponent* create(const char* name);
		void destroy(ToolComponent* component);

	public:
		ToolComponent* Tools[LocalToolFactory::max];

	private:
		Pool<ToolComponent, LocalToolFactory::max> mPool;

	};
}