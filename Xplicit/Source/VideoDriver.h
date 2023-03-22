/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: VideoDriver.h
 *			Purpose: Video Driver, abstracted Graphics back-end.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "DriverD3D11.h"

#ifdef XPLICIT_WINDOWS

namespace Xplicit
{
	namespace Renderer
	{
		class VideoDriver;
		class VideoBuffer;

		class XPLICIT_API VideoBuffer final
		{
		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
			D3D11_SUBRESOURCE_DATA m_resource_data;
			D3D11_BUFFER_DESC m_resource_desc;

			friend class VideoDriver;

		private:
			VideoBuffer() = default;

		public:
			~VideoBuffer();

			Microsoft::WRL::ComPtr<ID3D11Buffer>& get()
			{
				return m_buffer;
			}

			XPLICIT_COPY_DEFAULT(VideoBuffer);

		};

		/// <summary>
		/// An enum made to match the one from d3d11.
		/// </summary>
		enum VIDEO_USAGE
		{
			VIDEO_USAGE_DEFAULT = 0,
			VIDEO_USAGE_IMMUTABLE = 1,
			VIDEO_USAGE_DYNAMIC = 2,
			VIDEO_USAGE_STAGING = 3
		};

		class XPLICIT_API VideoDriver final
		{
		public:
			VideoDriver(HWND hwnd);
			~VideoDriver();

			VideoDriver& operator=(const VideoDriver&) = default;
			VideoDriver(const VideoDriver&) = default;

			VideoBuffer* make_buffer(enum VIDEO_USAGE videoUsage, 
				void* videoData = nullptr,
				size_t sz = 16);

		private:
			std::unique_ptr<DX11::DriverSystemD3D11> m_driver;
			HWND m_hwnd;

		};

		using VideoBufferPtr = std::unique_ptr<VideoBuffer>;
	}
}

#endif
