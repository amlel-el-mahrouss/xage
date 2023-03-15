/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: CoreUI.h
 *			Purpose: Core Client UI.
 *
 * =====================================================================
 */

#pragma once

#include <NetworkInstance.h>
#include <ApplicationContext.h>
#include <Instance.h>

#ifdef XPLICIT_WINDOWS
#include "DriverD3D11.h"
#endif

namespace Xplicit::CoreUI
{
	enum class ERROR_TYPE
	{
		NetworkError,
		Shutdown,
		Kicked,
	};

	class ErrorMessage final : public Instance
	{
	public:
		ErrorMessage() = delete;

	public:
		ErrorMessage(std::function<void()> on_click, const vector2di pos = vector2di(0, 0), const ERROR_TYPE shutdown_type = ERROR_TYPE::NetworkError) noexcept;
		virtual ~ErrorMessage();

		ErrorMessage& operator=(const ErrorMessage&) = delete;
		ErrorMessage(const ErrorMessage&) = delete;

		virtual void update() override;

		virtual const char* name() noexcept override;
		virtual INSTANCE_TYPE type() noexcept override;

	private:
		std::function<void()> m_on_click;
		ITexture* m_texture;
		vector2di m_pos;

	};

	class HUD final : public Instance
	{
	public:
		HUD();
		virtual ~HUD();

		HUD& operator=(const HUD&) = delete;
		HUD(const HUD&) = delete;

		virtual const char* name() noexcept override { return "HUD"; }
		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_GUI; }

		virtual void update() override;

	private:
		NetworkInstance* m_network;
		ITexture* m_texture;
		int64_t m_health;

	};
}