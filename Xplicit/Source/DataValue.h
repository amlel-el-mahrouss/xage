/*
 * =====================================================================
 *
 *			XplicitNgin C++ Game Engine
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include <Nplicit.h>

namespace Xplicit
{
	enum class DATA_VALUE_TYPE
	{
		BOOLEAN,
		STRING,
		INTEGER,
		ARRAY,
		NIL,
		IEE754, /* floating point number. */
	};

	class DataValue final
	{
	public:
		String Name;
		DATA_VALUE_TYPE Kind;
		std::vector<String> Values;

	};

	inline Vector<float> ToFloat(DataValue& pos_or_size)
	{
		try
		{
			float x = std::atof(pos_or_size.Values[0].c_str());
			float y = std::atof(pos_or_size.Values[1].c_str());
			float z = std::atof(pos_or_size.Values[2].c_str());

			return Vector<float>(x, y, z);
		}
		catch (const std::exception& error)
		{
			XPLICIT_INFO(error.what());

			return Vector<float>(0.f, 0.f, 0.f);
		}
	}

	inline bool ToBool(DataValue& boolean_type)
	{
		if (boolean_type.Values.empty())
			return false;

		if (boolean_type.Values[0] == "True")
			return true;
		else if (boolean_type.Values[0] == "False")
			return false;

		throw std::runtime_error("Invalid DataValue! Please select valid!");

		return false;
	}

	inline long ToInteger(DataValue& integer_type)
	{
		try
		{
			return std::atol(integer_type.Values[0].c_str());
		}
		catch (...)
		{
			return ~0;
		}
	}
}