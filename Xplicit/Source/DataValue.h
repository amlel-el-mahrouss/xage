/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: DataValue.h
 *			Purpose: Xplicit DataValue container.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit::Scene
{
	struct DataValue
	{
		char Name[64];
		char Value[64];
	};

	/*
	 *
	 * this should be called only for creating single data values.
	 * refer to Xplicit::Pool for multiple DataValues.
	 *
	 */

	inline std::unique_ptr<DataValue> make_data_value(const char* name, const char* value = "Nil") noexcept
	{
		DataValue* data_value = new DataValue();
		XPLICIT_ASSERT(data_value);

		memcpy(data_value->Name, name, strnlen(name, 64));
		memcpy(data_value->Value, value, strnlen(name, 64));

		return std::make_unique<DataValue>(data_value);
	}
}