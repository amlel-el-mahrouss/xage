/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Instance.h
 *			Purpose: Xplicit's ECS
 *
 * =====================================================================
 */

#include "Avx.h"

template <typename T, typename... Args>
T* Xplicit::InstanceManager::add(Args&&... args)
{
	T* ptr = new T{ args... };
	XPLICIT_ASSERT(ptr);

	m_instances.push_back(ptr);

	return ptr;
}

template <typename T>
T* Xplicit::InstanceManager::get(const char* name)
{
	if (!name ||
		*name == 0)
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
std::vector<T*> Xplicit::InstanceManager::all_of(const char* name)
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
bool Xplicit::InstanceManager::remove(T* ptr)
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
