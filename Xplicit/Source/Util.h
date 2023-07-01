/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Nplicit.h>

#include "Root.h"
#include "Xplicit.h"
#include "XplicitID.h"
#include "LuaScriptComponent.h"
#include "NetworkServerComponent.h"

inline Xplicit::Color<float> Magenta(0xFF, 0x00, 0xFF, 0xFF);
inline Xplicit::Color<float> White(0xFF, 0xFF, 0xFF, 0xFF);
inline Xplicit::Color<float> DarkRed(0x8B, 0, 0, 0xFF);
inline Xplicit::Color<float> Black(0, 0, 0, 0xFF);
inline Xplicit::Color<float> Red(0xFF, 0, 0, 0xFF);

inline Xplicit::Auth::XplicitID XPLICIT_INVALID_ID(0xFF, 0xFFFFFF);
inline Xplicit::Vector<float>   XPLICIT_ORIGIN(0.0f, 0.0f, 0.0f);

inline Xplicit::Auth::XplicitID& GetXplicitID(const std::size_t player_index)
{
	if (player_index > Xplicit::XPLICIT_MAX_CONNECTIONS)
		return XPLICIT_INVALID_ID;

	Xplicit::NetworkServerComponent* network = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	if (network)
		return network->get(player_index)->xplicit_id;

	return XPLICIT_INVALID_ID;
}

namespace Xplicit
{
	class XPLICIT_API XAttribute final
	{
	public:
		explicit XAttribute()
			: 
			  mScript(nullptr),
			  mColor(0.0f, 0.0f, 0.0f),
			  mPos(0.0f, 0.0f, 0.0f), 
			  mScale(0.0f, 0.0f, 0.0f),
			  mArchivable(false)
		{}

		~XAttribute() = default;

	public:
		XPLICIT_COPY_DEFAULT(XAttribute);

	public:
		bool insert(void* attribute) noexcept
		{
			if (attribute)
			{
				mAttributes.push_back(attribute);
				return true;
			}

			return false;
		}

		bool remove(void* attribute) noexcept
		{
			auto it = std::find(mAttributes.cbegin(), mAttributes.cend(), attribute);

			if (it != mAttributes.cend())
			{
				mAttributes.erase(it);
				return true;
			}

			return false;
		}

	public:
		Color<float>& color() noexcept { return mColor; }

		Vector<float>& scale() noexcept { return mScale; }

		Vector<float>& pos() noexcept { return mPos; }

		bool is_archivable() noexcept { return mArchivable; }

		LuaScriptComponent* script() noexcept { return mScript; }

		void set_script(LuaScriptComponent* pScript) noexcept 
		{
			if (pScript)
				mScript = pScript; 
		}

	private:
		std::vector<void*> mAttributes;

	private:
		LuaScriptComponent* mScript;
		Vector<float> mScale;
		Color<float> mColor;
		Vector<float> mPos;
		bool mArchivable;

	};
}