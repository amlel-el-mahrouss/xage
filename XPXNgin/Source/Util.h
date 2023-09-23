/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#ifdef __NPLICIT_DLL__
#	include "NMath.h"
#	include <NetworkProtocol.h>
#else
#	include <NMath.h>

#	include "Root.h"
#	include "XplicitID.h"
#	include "NetworkProtocol.h"
#	include "XHTTPManager.h"
#	include "NetworkServerComponent.h"

#	define XPLICIT_XASSET_URI "xasset://"
#	define XPLICIT_XASSET_URI_ENDPOINT "play-xplicit.com"

inline XPX::Color<XPX::NetworkFloat> Magenta(0xFF, 0x00, 0xFF, 0xFF);
inline XPX::Color<XPX::NetworkFloat> White(0xFF, 0xFF, 0xFF, 0xFF);
inline XPX::Color<XPX::NetworkFloat> DarkRed(0x8B, 0, 0, 0xFF);
inline XPX::Color<XPX::NetworkFloat> Black(0, 0, 0, 0xFF);
inline XPX::Color<XPX::NetworkFloat> Red(0xFF, 0, 0, 0xFF);

inline XPX::Auth::XplicitID XPLICIT_INVALID_ID(0xFF, UINT32_MAX);
inline XPX::Vector<XPX::NetworkFloat> XPLICIT_ORIGIN(0, 30, -40);

//! standard rig-5 character.
inline XPX::Vector<XPX::NetworkFloat> XPLICIT_CHARACTER_SCALE(3, 3, 1);

/// <summary>
/// Gets the XPLICIT_ID of a specific peer.
/// </summary>
/// <param name="player_index">player at index.</param>
/// <returns></returns>
inline XPX::Auth::XplicitID& GetXplicitID(const std::size_t player_index)
{
	if (player_index > XPX::XPLICIT_MAX_CONNECTIONS)
		return XPLICIT_INVALID_ID;

	XPX::NetworkServerComponent* network = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::NetworkServerComponent>("NetworkServerComponent");

	if (network)
		return network->get(player_index)->xplicit_id;

	return XPLICIT_INVALID_ID;
}

inline bool DownloadURL(std::string _url, std::string out_path) noexcept
{
	std::unique_ptr<XPX::XHTTPManager> http_manager = std::make_unique<XPX::XHTTPManager>();

	if (http_manager &&
		_url.find(XPLICIT_XASSET_URI) != XPX::String::npos)
	{
		XPX::String url = "/";
		url += _url.erase(_url.find(XPLICIT_XASSET_URI), strlen(XPLICIT_XASSET_URI));

		http_manager->set_endpoint(XPLICIT_XASSET_URI_ENDPOINT);

		return http_manager->download(url, out_path);
	}

	return false;
}

#endif // ifdef __NPLICIT_DLL__

namespace XPX
{
	class LuaScriptComponent;

	/// <summary>
	/// Xplicit Attribute Class.
	/// Reserved for RoXML and scripting API.
	/// </summary>
	class XPXAttribute
	{
	public:
		XPXAttribute()
			: 
			  mScript(nullptr),
			  mColor(0.0f, 0.0f, 0.0f),
			  mPos(0.0f, 0.0f, 0.0f), 
			  mScale(0.0f, 0.0f, 0.0f),
			  mRot(0.0f, 0.0f, 0.0f),
			  mArchivable(false),
			  mLocked(false),
			  mAlpha(1.0f),
			  mNoCollide(false)
		{}

		virtual ~XPXAttribute() = default;

	public:
		XPLICIT_COPY_DEFAULT(XPXAttribute);

	public:
		Color<NetworkFloat>& color() noexcept { return mColor; }
		Vector<NetworkFloat>& rotation() noexcept { return mRot; }
		Vector<NetworkFloat>& scale() noexcept { return mScale; }
		Vector<NetworkFloat>& pos() noexcept { return mPos; }

	public:
		LuaScriptComponent* script() noexcept { return mScript; }
		bool is_archivable() noexcept { return mArchivable; }
		bool has_no_collide() noexcept { return !mNoCollide; }
		bool is_locked() noexcept { return mLocked; }
		bool alpha() noexcept { return mLocked; }

	public:
		void script(LuaScriptComponent* script) noexcept { mScript = script; }
		void archivable(const bool enable) noexcept { mArchivable = enable; }
		void collide(const bool enable) noexcept { mNoCollide = enable; }
		void locked(const bool enable) noexcept { mLocked = enable; }
		void alpha(const float alpha) noexcept { mAlpha = alpha;  }

	private:
		LuaScriptComponent* mScript;
		Vector<NetworkFloat> mScale;
		Color<NetworkFloat> mColor;
		Vector<NetworkFloat> mRot;
		Vector<NetworkFloat> mPos;

	private:
		bool mArchivable;
		bool mNoCollide;
		bool mLocked;
		float mAlpha;

	};
}

#include "Uri.h"
#include "LuaAPI.h"