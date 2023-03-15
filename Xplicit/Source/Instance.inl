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
	assert(ptr);

	m_instances.push_back(ptr);

	return ptr;
}

template <typename T>
T* Xplicit::InstanceManager::get(const char* name)
{
	if (!name)
		return nullptr;

	if (*name == 0)
		return nullptr;

	for (size_t i = 0; i < m_instances.size(); ++i)
	{
		if (!m_instances[i])
			continue;

#ifdef XPLICIT_USE_AVX
		if (avx_strcmp(name, m_instances[i]->name()))
#else
		if (strcmp(name, m_instances[i]->name()) == 0)
#endif
			return static_cast<T*>(m_instances[i]);
	}

	return nullptr;
}

template <typename T>
std::vector<T*> Xplicit::InstanceManager::get_all(const char* name)
{
	if (!name)
		return {};

	if (*name == 0)
		return {};

	std::vector<T*> list;

	for (size_t i = 0; i < m_instances.size(); ++i)
	{
		if (!m_instances[i])
			continue;

#ifdef XPLICIT_USE_AVX
		if (avx_strcmp(name, m_instances[i]->name()))
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

	auto element = std::find(m_instances.cbegin(), m_instances.cend(), ptr);

	if (element != m_instances.cend())
	{
		m_instances.erase(element);
		delete ptr;

		return true;
	}

	return false;
}

template <typename T>
bool Xplicit::InstanceManager::remove(const char* name)
{
	if (!name)
		return false;

	if (*name == 0)
		return false;

	for (size_t it = 0; it < m_instances.size(); ++it)
	{
#ifdef XPLICIT_USE_AVX
		if (m_instances[it] && avx_strcmp(name, m_instances[it]->name()))
#else
		if (m_instances[it] && strcmp(name, m_instances[it]->name()) == 0)
#endif
		{
			auto obj = m_instances[it];

			auto element = std::find(m_instances.cbegin(), m_instances.cend(), m_instances[it]);

			if (element != m_instances.cend())
			{
				m_instances.erase(element);

				// finally remove the object
				delete obj;

				return true;
			}

			break;
		}
	}

	return false;
}
