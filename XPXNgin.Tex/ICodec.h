/*
 * =====================================================================
 *
 *			XAGE.Tex
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			Purpose: Codec streams.
 *
 * =====================================================================
 */

#pragma once

namespace XPX::Renderer
{
	class ICodec
	{
	public:
		explicit ICodec() { this->on_setup(); this->on_post_setup(); }
		virtual ~ICodec() { this->on_clean(); }

	public:
		ICodec& operator=(const ICodec&) = default;
		ICodec(const ICodec&) = default;

	public:
		virtual bool frame() = 0;

	public:
		virtual void on_setup() = 0;
		virtual void on_clean() = 0;

	public:
		virtual void on_post_setup() = 0;
		virtual void on_fatal_error() = 0;

	protected:
		bool m_bLoop{ false };

	};
}