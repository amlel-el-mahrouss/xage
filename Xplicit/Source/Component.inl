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
T* Xplicit::ComponentSystem::add(Args&&... args)
{
	if (T* ptr = new T(std::forward<Args>(args)...); ptr)
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Created component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif

		mComponents.push_back((Component*)ptr);
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::ComponentSystem::get(const char* name) noexcept
{
	if (!name || *name == 0)
		return nullptr;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i])
			continue;

		if (strcmp(name, mComponents[i]->name()) == 0)
			return (T*)mComponents[i];
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::ComponentSystem::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		if (!mComponents[i])
			continue;

		// move that to upper file if you happen to use that in eveyr part of the file.
		if (strcmp(name, mComponents[i]->name()) == 0)
		{
			list.push_back((T*)mComponents[i]);
		}
	}

	return list;
}

template <typename T>
bool Xplicit::ComponentSystem::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find_if(mComponents.begin(), mComponents.end(), [&](Component* component) -> bool {
		return component == ptr;
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
