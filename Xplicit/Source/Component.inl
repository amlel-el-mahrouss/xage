/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */


template <typename T, typename... Args>
T* Xplicit::ComponentManager::add(Args&&... args)
{
	T* ptr = new T(std::forward<Args>(args)...);

	if (ptr)
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Created component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif

		mComponents.push_back(reinterpret_cast<Component*>(ptr));
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::ComponentManager::get(const char* name) noexcept
{
	if (!name || *name == 0)
		return nullptr;

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, mComponents[i]->name()))
#else
		if (strcmp(name, mComponents[i]->name()) == 0)
#endif
			return static_cast<T*>(mComponents[i]);
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::ComponentManager::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i])
			continue;

		// move that to upper file if you happen to use that in eveyr part of the file.
#ifdef XPLICIT_USE_VECTOR
#	include "Avx.h"
		if (avx_strequals(name, mComponents[i]->name()))
#else
		if (strcmp(name, mComponents[i]->name()) == 0)
#endif
		{
			list.push_back(static_cast<T*>(mComponents[i]));
		}
	}

	return list;
}

template <typename T>
bool Xplicit::ComponentManager::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find(mComponents.cbegin(), mComponents.cend(), ptr);

	if (iterator != mComponents.cend())
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Destroyed component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif

		mComponents.erase(iterator);

		delete ptr;

		return true;
	}

	return false;
}
