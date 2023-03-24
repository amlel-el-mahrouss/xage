/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: VideoDriver.cpp
 *			Purpose: Direct3D renderer.
 *
 * =====================================================================
 */

#include "VideoDriver.h"

namespace Xplicit::Renderer
{
	VideoBuffer::~VideoBuffer()
	{
		if (m_buffer)
			m_buffer->Release();

		delete this;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer>& VideoBuffer::get()
	{
		return m_buffer;
	}

	VideoDriver::VideoDriver(HWND hwnd)
		: m_driver(DX11::make_driver_system_d3d11(hwnd)), m_hwnd(hwnd)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("VideoDriver::VideoDriver");
#endif
	}

	VideoDriver::~VideoDriver()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("VideoDriver::~VideoDriver");
#endif
	}

	VideoBuffer* VideoDriver::make_buffer(enum VIDEO_USAGE usage, void* data, size_t sz)
	{
		VideoBuffer* buf = new VideoBuffer();
		XPLICIT_ASSERT(buf);

		if (!buf)
			return nullptr;

		RtlZeroMemory(&buf->m_resource_desc, sizeof(D3D11_SUBRESOURCE_DATA));

		buf->m_resource_desc.Usage = (D3D11_USAGE)usage;
		buf->m_resource_desc.ByteWidth = sz;

		RtlZeroMemory(&buf->m_resource_data, sizeof(D3D11_SUBRESOURCE_DATA));
		buf->m_resource_data.pSysMem = data;

		HRESULT hr = m_driver->get().Device->CreateBuffer(&buf->m_resource_desc, &buf->m_resource_data, &buf->m_buffer);

		if (SUCCEEDED(hr))
			return buf;
	
		delete buf;

		return nullptr;
	}
}