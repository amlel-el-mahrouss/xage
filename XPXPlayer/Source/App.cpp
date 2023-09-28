/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "App.h"
#include "LoadingScreenComponent.h"

namespace XPX::Bites
{
	ApplicationManager::ApplicationManager(Utils::UriParser xconnect_to)
		: mPath(""),
		mWsa(),
		mRenderer(nullptr),
		mRenderer2D(nullptr),
		mRenderingWindow(nullptr)
	{
		this->setup_engine();

		ComponentSystem::get_singleton_ptr()->add<NetworkComponent>();

		const auto loading_screen = ComponentSystem::get_singleton_ptr()->add<LoadingScreenComponent>();
		XPLICIT_ASSERT(loading_screen);

		loading_screen->connect(xconnect_to);
	}

	ApplicationManager::~ApplicationManager() {}

	void ApplicationManager::setup_engine()
	{
		//! Setup program contents path.
		XPLICIT_GET_DATA_DIR(path);
		mPath = path;

		//! Setup SettingsManager
		mSettings = std::make_unique<SettingsManager>();
		XPLICIT_ASSERT(mSettings);

		SettingsManager::Traits traits;

		(*mSettings) >> traits;

		if (traits.window_height < 600 ||
			traits.window_width < 800)
			throw EngineError("XAGE doesn't support DPI displays.");

#ifdef XPLICIT_WINDOWS
		XPX::init_winsock(&mWsa);
#endif

		//! Fetch our file first.
		String path_ini(path);
		path_ini += "XAGE.cfg";

		//! Check for the existence of this ini file.
		if (!std::filesystem::exists(path_ini))
		{
			traits.window_width = XPLICIT_MIN_WIDTH;
			traits.window_height = XPLICIT_MIN_HEIGHT;
			traits.mouse_sensitivity = 1.0f;

			(*mSettings) << traits;
		}

		mRenderingWindow = std::make_unique<Win32Window>("XPX Advanced Game Engine", "XAGE", GetModuleHandle(nullptr));
		
		mRenderer = std::make_unique<Renderer::DX11::DriverSystemD3D11>(mRenderingWindow->get().WindowHandle);
		mRenderer2D = std::make_unique<Renderer::D2D::DriverSystemD2D>(mRenderer.get());

		D3D11_TEXTURE2D_DESC texDesc{};
		texDesc.ArraySize = 1;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.Height = traits.window_height;
		texDesc.Width = traits.window_height;
		texDesc.MipLevels = 1;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;

		HRESULT hr = mRenderer->get().pDevice->CreateTexture2D(&texDesc, nullptr, &mRenderer2D->f_pTexture);

		Renderer::DX11::Details::ThrowIfFailed(hr);

		IDXGISurface* pDxgiSurface = nullptr;

		hr = mRenderer2D->f_pTexture->QueryInterface(&pDxgiSurface);

		Renderer::DX11::Details::ThrowIfFailed(hr);
		
		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
				96,
				96);

		hr = mRenderer2D->f_pDirect2dFactory->CreateDxgiSurfaceRenderTarget(
			pDxgiSurface,
			&props,
			mRenderer2D->f_pRenderTarget.GetAddressOf());

		Renderer::DX11::Details::ThrowIfFailed(hr);

		mRenderer2D->f_pDxgiSurface = pDxgiSurface;

		Root::get_singleton_ptr()->set(mRenderer.get());
		Root::get_singleton_ptr()->set(mRenderer2D.get());
		Root::get_singleton_ptr()->set(new InputReceiver());
	}

	ApplicationManager::SettingsManager::SettingsManager()
		: mSettingsPath()
	{
		XPLICIT_GET_DATA_DIR(dat);

		mSettingsPath = dat;
		mSettingsPath += "ClientSettings.ini";

		mIni = std::make_unique<mINI::INIFile>(mSettingsPath);
	}

	ApplicationManager::SettingsManager::~SettingsManager() = default;

	ApplicationManager::SettingsManager& ApplicationManager::SettingsManager::operator <<(Traits& traits)
	{
		mINI::INIStructure ini;

		ini["Window"]["Width"] = std::to_string(traits.window_width);
		ini["Window"]["Height"] = std::to_string(traits.window_height);

		mIni->write(ini, true);

		return *this;
	}

	/* reads the ClientSettings.dat INI file */
	ApplicationManager::SettingsManager& ApplicationManager::SettingsManager::operator >>(Traits& traits)
	{
		try
		{
			mINI::INIStructure ini;
			if (!mIni->read(ini))
				throw EngineError("File not found!");

			const auto width_int = std::atoi(ini["Window"]["Width"].c_str());
			const auto height_int = std::atoi(ini["Window"]["Height"].c_str());

			traits.window_width = width_int;
			traits.window_height = height_int;
		}
		catch (...)
		{
			XPLICIT_CRITICAL("NOT FOUND! Configuration file for XAGE wasn't found!");
		}

		if (traits.window_width < XPLICIT_DEFAULT_WIDTH ||
			traits.window_height < XPLICIT_DEFAULT_HEIGHT)
		{
			traits.window_width = XPLICIT_DEFAULT_WIDTH;
			traits.window_height = XPLICIT_DEFAULT_HEIGHT;
		}

		return *this;
	}
}