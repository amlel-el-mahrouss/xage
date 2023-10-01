/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#ifdef XPLICIT_USE_AVX
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

		accessor._AbiName = typeid(T).name();
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
	if (!name || 
		*name == 0)
		return nullptr;

	std::vector<Details::ComponentAccessor>::const_iterator it = std::find_if(mComponents.cbegin(), 
		mComponents.cend(), 
		[&](const Details::ComponentAccessor comp) -> bool {
		return name == comp.name();
	});

	if (it != mComponents.cend())
	{
		return it->as_type<T*>();
	}

	return nullptr;
}

inline constexpr int XPX_MAX_RESERVATIONS = 30;

template <typename T>
std::vector<T*> XPX::ComponentSystem::all_of()
{
	std::vector<T*> list;
	list.reserve(XPX_MAX_RESERVATIONS);

	for (std::size_t i = 0; i < mComponents.size(); ++i)
	{
		auto& comp = mComponents[i];

		if (typeid(T).name() != comp._AbiName)
			continue;

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


template <typename T>
bool XPX::ComponentSystem::erase(std::vector<Details::ComponentAccessor>::const_iterator& it)
{
	if (it != mComponents.cend())
	{
#ifdef XPLICIT_DEBUG
		String fmt = "Destroyed component: ";
		fmt += typeid(T).name();

		XPLICIT_INFO(fmt);
#endif

		delete it->as_type<T*>();
		mComponents.erase(it);

		return true;
	}

	return false;
}

inline std::vector<XPX::Details::ComponentAccessor>& XPX::ComponentSystem::get_all() noexcept { return mComponents; }