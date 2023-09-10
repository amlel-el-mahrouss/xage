/*
 * =====================================================================
 *
 *			XplicitNgin C++ World Engine
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#ifdef XPLICIT_USE_VECTOR
#	include "Avx.h"
#endif

template <typename T, typename... Args>
T* XPX::ComponentSystem::add(Args&&... args)
{
	T* ptr = new T(std::forward<Args>(args)...);

	if (ptr)
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Created component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif
		Details::ComponentAccessor accessor;

		accessor._Name = ptr->name();
		accessor._Pointee = reinterpret_cast<void*>(ptr);
		accessor._Update = &T::update;
		accessor._Eval = &T::should_update;

		mComponents.push_back(accessor);

		return ptr;
	}

	return nullptr;
}

template <typename T>
T* XPX::ComponentSystem::get(const char* name)
{
	if (!name || *name == 0)
		return nullptr;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		auto& comp = mComponents[i];

		if (!comp.as_type<T*>())
			continue;

		if (name == comp._Name)
			return comp.as_type<T*>(); //! __thiscall wants the class at ECX.
	}

	return nullptr;
}

template <typename T>
std::vector<T*> XPX::ComponentSystem::all_of(const char* name)
{
	std::vector<T*> list;

	if (!name || *name == 0)
		return list;

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		auto& comp = mComponents[i];

		if (!comp.as_type<T*>())
			continue;

		if (name == comp._Name)
			list.push_back(comp.as_type<T*>());
	}

	return list;
}

template <typename T>
bool XPX::ComponentSystem::remove(T* ptr)
{
	if (!ptr)
		return false;

	auto iterator = std::find_if(mComponents.begin(), mComponents.end(), [&](Details::ComponentAccessor& accessor) -> bool {
		return accessor.as_type<T*>() == ptr;
	});

	if (iterator != mComponents.end())
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Destroyed component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif

		delete ptr;
		mComponents.erase(iterator);

		return true;
	}

	return false;
}