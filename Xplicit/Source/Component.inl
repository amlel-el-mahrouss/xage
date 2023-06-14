/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Component.h
 *			Purpose: Xplicit's Component System
 *
 * =====================================================================
 */

#include "Avx.h"

template <typename T, typename... Args>
T* Xplicit::ComponentManager::add(Args&&... args)
{
	T* ptr = new T{ args... };
	XPLICIT_ASSERT(ptr);

	if (ptr)
	{
		m_instances.push_back(reinterpret_cast<Component*>(ptr));
		return ptr;
	}

	return nullptr;
}

template <typename T>
T* Xplicit::ComponentManager::get(const char* name)
{
	if (!name || *name == 0)
		return nullptr;

	for (size_t i = 0; i < m_instances.size(); ++i)
	{
		if (!m_instances[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, m_instances[i]->name()))
#else
		if (strcmp(name, m_instances[i]->name()) == 0)
#endif
			return static_cast<T*>(m_instances[i]);
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::ComponentManager::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (size_t i = 0; i < m_instances.size(); ++i)
	{
		if (!m_instances[i])
			continue;

#ifdef XPLICIT_USE_VECTOR
		if (avx_strequals(name, m_instances[i]->name()))
#else
		if (strcmp(name, m_instances[i]->name()) == 0)
#endif
			list.push_back(static_cast<T*>(m_instances[i]));
	}

	return list;
}

template <typename T>
bool Xplicit::ComponentManager::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find(m_instances.cbegin(), m_instances.cend(), ptr);

	if (iterator != m_instances.cend())
	{
		m_instances.erase(iterator);
		delete ptr;

		return true;
	}

	return false;
}
