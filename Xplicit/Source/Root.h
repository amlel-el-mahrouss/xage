/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <IEventReceiver.h>

/* common engine macros for Root */
#define RENDER  Xplicit::Root::get_singleton_ptr()->Dev

namespace Xplicit
{
	class XPLICIT_API Root final
	{
	private:
		explicit Root()
			: Keyboard(nullptr), ShouldExit(false), Dev(nullptr)
		{}

		~Root() noexcept
		{
			if (Dev)
				Dev->drop();

			ShouldExit = true;
		}

		Root& operator=(const Root&) = default;
		Root(const Root&) = default;

	public:
		static Root* get_singleton_ptr() noexcept
		{
			static Root* root = nullptr;

			if (!root)
				root = new Root();

			return root;
		}

	public:
		void set(irr::IrrlichtDevice* dev) noexcept
		{
			if (dev)
				Dev = dev;
		}

		void set(IEventReceiver* kb) noexcept
		{
			if (kb)
				Keyboard = kb;
		}

	public:
		IEventReceiver* Keyboard;
		IrrlichtDevice* Dev;
		bool ShouldExit;

	};
}

#include "HelperMacros.h"

