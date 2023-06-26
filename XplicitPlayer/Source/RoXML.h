/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* RoXML format */

#include <Gears.h>
#include <filesystem>
#include <DataValue.h>
#include <LuaScriptComponent.h>

#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>

namespace Xplicit::SceneManager
{
	using namespace rapidxml;

	class RoXMLDocumentParser final
	{
	public:
		RoXMLDocumentParser() = default;
		~RoXMLDocumentParser() = default;

	public:
		XPLICIT_COPY_DEFAULT(RoXMLDocumentParser);

	public:
		void load_scene(String& path) noexcept
		{
			if (path.empty() ||
				!std::filesystem::exists(path))
				return;

			rapidxml::file<> xml_file(path.c_str()); // Default template is char
			rapidxml::xml_document<> doc;

			doc.parse<0>(xml_file.data());

			xml_node<>* node = doc.first_node("Xplicit");

			for (xml_attribute<>* attr = node->first_attribute();
				attr; attr = attr->next_attribute())
			{
				std::cout << attr->name() << std::endl;
			}
		}

	};
}