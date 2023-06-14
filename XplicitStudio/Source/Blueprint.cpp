/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Blueprint.cpp
 *			Purpose: C++ Blueprints
 *
 * =====================================================================
 */

#include "Blueprint.h"

namespace Xplicit::Studio
{
	Blueprint::Blueprint() : mPrev(nullptr), mNext(nullptr) {}

	Blueprint::~Blueprint()
	{
		if (mNext)
			mNext->detach(this);

		if (mPrev)
			mPrev->detach(this);
	}

	void Blueprint::detach(Blueprint* pBlueprint)
	{
		if (!pBlueprint)
			return;

		Blueprint* curBlueprint = mNext;

		bool found = false;

		Thread thrd = Thread([&]() -> void {
			while (!found)
			{
				if (curBlueprint)
				{
					if (curBlueprint == pBlueprint)
					{
						curBlueprint->mNext->mPrev = curBlueprint->mPrev;
						curBlueprint->mPrev->mNext = curBlueprint->mNext;

						found = true;
					}

					curBlueprint = curBlueprint->mPrev;

					continue;
				}

				break;
			}
		});

		thrd.detach();
	}

	void Blueprint::attach(Blueprint* pBlueprint)
	{
		auto* prev = mNext;
		
		mNext = pBlueprint;

		mNext->mPrev = prev;
		mNext->mNext = nullptr;
	}
}