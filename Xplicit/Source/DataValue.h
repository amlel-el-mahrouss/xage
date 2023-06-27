/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit
{
	enum class DATA_VALUE_TYPE
	{
		BOOLEAN,
		STRING,
		INTEGER,
		NIL,
		IEE754, /* floating point number. */
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

	inline DataValue* make_data_value(const char* name, const char* value = "Nil", const DATA_VALUE_TYPE type = DATA_VALUE_TYPE::NIL) noexcept
	{
		DataValue* data_value = new DataValue();

		if (data_value == nullptr)
			throw EngineError("Could not allocate DataValue (probably out of memory?)");

		XPLICIT_ASSERT(data_value);

		data_value->Type = type;
		data_value->Name = name;
		data_value->Value = value;

		return data_value;
	}
}