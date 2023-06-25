/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

template <typename T, typename... Args>
T* Xplicit::EventManager::add(Args&&... args)
{
	T* ptr = new T{ args... };
	XPLICIT_ASSERT(ptr);

	if (ptr)
	{
		mEvents.push_back(reinterpret_cast<Event*>(ptr));
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::EventManager::get(const char* name)
{
	for (size_t i = 0; i < mEvents.size(); i++)
	{
		if (!mEvents[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
#	include "Avx.h"

		if (avx_strequals(name, mEvents[i]->name()))
#else
		if (strcmp(name, mEvents[i]->name()) == 0)
#endif
			return static_cast<T*>(mEvents[i]);
	}

	return nullptr;
}

template <typename T>
bool Xplicit::EventManager::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find(mEvents.cbegin(), mEvents.cend(), ptr);

	if (iterator != mEvents.cend())
	{
		mEvents.erase(iterator);
		delete ptr;

		return true;
	}

	return false;
}