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

//! XPLICIT lobby ticket.
inline XPX::Auth::XplicitID XPLICIT_INVALID_ID(0xFF, UINT32_MAX);

//! World origin.
inline XPX::Vector<XPX::NetworkFloat> XPLICIT_ORIGIN(0, 0, -10);

//! a perfect octree, where all of the details are rendered, when getting out of it, 
//! they fadeout and only low poly versions or polygons are shown.
inline XPX::Vector<XPX::NetworkFloat> XPLICIT_PERFECT_OCTREE_SIZE(64, 64, 64);

/// <summary>
/// Gets the XPLICIT_ID of a specific peer.
/// </summary>
/// <param name="player_index">player at index.</param>
/// <returns>It's xplicit id.</returns>
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
	class XPXAttribute
	{
	public:
		XPXAttribute()
			: 
			  mColor(0.0f, 0.0f, 0.0f),
			  mPos(0.0f, 0.0f, 0.0f), 
			  mScale(0.0f, 0.0f, 0.0f),
			  mRot(0.0f, 0.0f, 0.0f),
			  mArchivable(false),
			  mLocked(false),
			  mAlpha(1.0f),
			  mNoCollide(false),
			  mAnchor(true),
			  mLookPos(0.f, 0.f, 0.f)
		{}

		virtual ~XPXAttribute() = default;

	public:
		XPLICIT_COPY_DEFAULT(XPXAttribute);

	public:
		Color<NetworkFloat>& color() noexcept { return mColor; }
		Vector<NetworkFloat>& rotation() noexcept { return mRot; }
		Vector<NetworkFloat>& scale() noexcept { return mScale; }
		Vector<NetworkFloat>& look_pos() noexcept { return mLookPos; }
		Vector<NetworkFloat>& pos() noexcept { return mPos; }

	public:
		bool is_archivable() noexcept { return mArchivable; }
		bool has_no_collide() noexcept { return !mNoCollide; }
		bool is_locked() noexcept { return mLocked; }
		float alpha() noexcept { return mAlpha; }
		bool anchor() noexcept { return mAnchor; }

	public:
		void archivable(const bool enable) noexcept { mArchivable = enable; }
		void collide(const bool enable) noexcept { mNoCollide = enable; }
		void locked(const bool enable) noexcept { mLocked = enable; }
		void alpha(const float alpha) noexcept { mAlpha = alpha;  }
		void anchor(const bool enable) noexcept { mAnchor = enable; }

	public:
		std::size_t f_SceneId{ 0 };
		void* f_PhysicsDelegate{ nullptr };

	private:
		Vector<NetworkFloat> mLookPos;
		Vector<NetworkFloat> mScale;
		Color<NetworkFloat> mColor;
		Vector<NetworkFloat> mRot;
		Vector<NetworkFloat> mPos;

	private:
		bool mArchivable;
		bool mNoCollide;
		bool mAnchor;
		bool mLocked;
		float mAlpha;

	};
}

#include "Uri.h"