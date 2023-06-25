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

namespace Xplicit
{
	enum class DATA_VALUE_TYPE
	{
		BOOLE,
		STRING,
		INTEGER,
		IEE754,
	};

	class DataValue final
	{
	public:
		String Name;
		String Value;
		DATA_VALUE_TYPE Type;

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

		if (data_value == nullptr)
			throw EngineError("Invalid DataValue (probably out of memory?)");

		XPLICIT_ASSERT(data_value);

		data_value->Name = name;
		data_value->Value = value;

		return std::make_unique<DataValue>(data_value);
	}
}