/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include <CLua/CLua.hpp>

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "World."

namespace Xplicit
{
	SpawnComponent::SpawnComponent(const Vector<float>& vec) 
		: Component(), mClass(std::make_unique<Lua::CLuaClass>("SpawnLocation"))
	{
		XPLICIT_ASSERT(mClass);
		mClass->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
	}

	SpawnComponent::~SpawnComponent() = default;

	Vector<float>& SpawnComponent::get() noexcept { return mAttribute.pos(); }

	void SpawnComponent::update(void* class_ptr) 
	{
		SpawnComponent* _this = (SpawnComponent*)class_ptr;

		auto str = "{" + std::to_string(_this->mAttribute.pos().X) + "," +
			std::to_string(_this->mAttribute.pos().Y) + "," +
			std::to_string(_this->mAttribute.pos().Z) + "," + "}";

		_this->mClass->assign("Position", str.c_str());
	}

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }

	XAttribute& SpawnComponent::get_attribute() noexcept { return mAttribute; }
}