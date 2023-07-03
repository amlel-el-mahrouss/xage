/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
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

/// <summary>
/// Gets the XPLICIT_ID of a specific peer.
/// </summary>
/// <param name="player_index">player at index.</param>
/// <returns></returns>
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
	/// <summary>
	/// Xplicit Attribute Class.
	/// Reserved for RoXML and the Scripting/Engine APIs.
	/// </summary>
	class XAttribute final
	{
	public:
		explicit XAttribute()
			: 
			  mScript(nullptr),
			  mColor(0.0f, 0.0f, 0.0f),
			  mPos(0.0f, 0.0f, 0.0f), 
			  mScale(0.0f, 0.0f, 0.0f),
			  mArchivable(false),
			  mLocked(false),
			  mAlpha(1.0f),
			  mAnchored(false),
			  mNoCollide(false)
		{}

		~XAttribute() = default;

	public:
		XPLICIT_COPY_DEFAULT(XAttribute);

	public:
		Color<float>& color() noexcept { return mColor; }
		Vector<float>& scale() noexcept { return mScale; }
		Vector<float>& pos() noexcept { return mPos; }

	public:
		LuaScriptComponent* script() noexcept { return mScript; }
		bool is_archivable() noexcept { return mArchivable; }
		bool has_no_collide() noexcept { return mNoCollide; }
		bool is_anchored() noexcept { return mAnchored; }
		bool is_locked() noexcept { return mLocked; }
		bool alpha() noexcept { return mLocked; }

	public:
		void script(LuaScriptComponent* script) noexcept { mScript = script; }
		void archivable(const bool enable) noexcept { mArchivable = enable; }
		void collide(const bool enable) noexcept { mNoCollide = enable; }
		void anchor(const bool enable) noexcept { mAnchored = enable; }
		void locked(const bool enable) noexcept { mLocked = enable; }
		void alpha(const float alpha) noexcept { mAlpha = alpha;  }

	private:
		LuaScriptComponent* mScript;
		Vector<float> mScale;
		Color<float> mColor;
		Vector<float> mPos;

	private:
		bool mArchivable;
		bool mAnchored;
		bool mNoCollide;
		bool mLocked;
		float mAlpha;

	};
}