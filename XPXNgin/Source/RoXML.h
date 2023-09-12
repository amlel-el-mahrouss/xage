/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* RoXML format */
/* This format describes scene-nodes and stuff like that. */

// XML
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>

// Common includes
#include "LuaScriptComponent.h"
#include "ClassComponent.h"
#include "XHTTPManager.h"
#include "DataValue.h"
#include "Root.h"
#include "Util.h"

// CLua
#include <CLua.hpp>

#ifndef XPLICIT_XASSET_ENDPOINT
#	define XPLICIT_XASSET_ENDPOINT "play-xplicit.com"
#endif // ifndef XPLICIT_XASSET_ENDPOINT

namespace XPX::RoXML
{
	using namespace rapidxml;

	struct RoXMLNodeDescription final
	{
		explicit RoXMLNodeDescription()
			: 
			Color(0.0f, 0.0f, 0.0f),
			Size(0.0f, 0.0f, 0.0f),
			Position(0.0f, 0.0f, 0.0f),
			Rotation(0.0f, 0.0f, 0.0f, 0.0f),
			Value(""),
			Name(""),
			ID("")
		{}

		String ID;
		String Name;
		String Value;
		Color<float> Color;
		Vector<float> Size;
		Vector<float> Position;
		Quaternion<float> Rotation;

	};

	struct RoXMLDocumentParameters
	{
		String Path{ "" };

		bool Inline{ false };
		bool Has3D{ false };
		bool LuaOnly{ false };
		bool NoLua{ false };
		bool WaitFor{ false };

		std::vector<DataValue> DataValues;
		std::vector<RoXMLNodeDescription> WorldNodes;

	};

	class XPLICIT_API RoXMLDocumentParser final
	{
	public:
		RoXMLDocumentParser() = default;
		~RoXMLDocumentParser() = default;

	public:
		XPLICIT_COPY_DEFAULT(RoXMLDocumentParser);

	public:
		void parse(RoXMLDocumentParameters& params) noexcept;

	};
}

// Utilities related to data values.

XPLICIT_API void DVFromRoXML(XPX::RoXML::RoXMLDocumentParameters& params) noexcept;