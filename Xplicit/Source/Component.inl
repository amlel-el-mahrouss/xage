/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Component.h
 *			Purpose: Xplicit Component System and memory pools
 *
 * =====================================================================
 */

#include "Avx.h"

template <typename T, typename... Args>
T* Xplicit::ComponentManager::add(Args&&... args)
{
	T* ptr = new T(std::forward<Args>(args)...);

	if (ptr)
	{
		mInstances.push_back(reinterpret_cast<Component*>(ptr));
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::ComponentManager::get(const char* name)
{
	if (!name || *name == 0)
		return nullptr;

	for (size_t i = 0; i < mInstances.size(); ++i)
	{
		if (!mInstances[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, mInstances[i]->name()))
#else
		if (strcmp(name, mInstances[i]->name()) == 0)
#endif
			return static_cast<T*>(mInstances[i]);
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::ComponentManager::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (size_t i = 0; i < mInstances.size(); ++i)
	{
		if (!mInstances[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, mInstances[i]->name()))
#else
		if (strcmp(name, mInstances[i]->name()) == 0)
#endif
		{
			list.push_back(static_cast<T*>(mInstances[i]));
		}
	}

	return list;
}

template <typename T>
bool Xplicit::ComponentManager::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find(mInstances.cbegin(), mInstances.cend(), ptr);

	if (iterator != mInstances.cend())
	{
		mInstances.erase(iterator);
		delete ptr;

		return true;
	}

	return false;
}
