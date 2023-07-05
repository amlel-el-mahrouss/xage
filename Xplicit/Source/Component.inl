/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#ifdef XPLICIT_USE_VECTOR
#	include "Avx.h"
#endif

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

		Detail::ComponentAccessor accessor;
		accessor._Pointee = reinterpret_cast<std::uintptr_t>(ptr);

		mComponents.push_back(accessor);
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::ComponentManager::get(const char* name) noexcept
{
	if (!name || *name == 0)
		return nullptr;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i].as_type<T*>())
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, mComponents[i].as_type<T>()->name()))
#else
		if (strcmp(name, mComponents[i].as_type<T*>()->name()) == 0)
#endif
			return mComponents[i].as_type<T*>();
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::ComponentManager::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i].as_type<T*>())
			continue;

		// move that to upper file if you happen to use that in eveyr part of the file.
#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, mComponents[i].as_type<T>()->name()))
#else
		if (strcmp(name, mComponents[i].as_type<T*>()->name()) == 0)
#endif
		{
			list.push_back(mComponents[i].as_type<T*>());
		}
	}

	return list;
}

template <typename T>
bool Xplicit::ComponentManager::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find_if(mComponents.begin(), mComponents.end(), [&](Detail::ComponentAccessor& accessor) -> bool {
		return accessor.as_type<T*>() == ptr;
	});

	if (iterator != mComponents.end())
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
