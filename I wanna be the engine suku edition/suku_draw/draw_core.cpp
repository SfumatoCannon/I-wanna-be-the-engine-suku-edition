#include "pch.h"
#include "draw_core.h"
#include "includes.h"
#include "../suku_foundation/includes.h"
#include "d3d_draw_core.h"

namespace suku
{
//#ifdef USE_D3D11
//	ComPtr<ID2D1Device> pD2DDevice = nullptr;
//	ComPtr<ID2D1DeviceContext> pD2DContext = nullptr;
//#else
	ComPtr<ID2D1HwndRenderTarget> pMainRenderTarget = nullptr;	// Render target
//#endif
	void suku_drawing_preinit_WIC(ComPtr<IWICImagingFactory>* ppWICFactory)
	{
		// Init WIC resource
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (SUCCEEDED(hr))
		{
			ComPtr<IWICImagingFactory> localFactory;
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&localFactory)
			);
			if (SUCCEEDED(hr) && ppWICFactory)
			{
				*ppWICFactory = std::move(localFactory);
			}
		}
		else
		{
			ERRORWINDOW_GLOBAL("Failed to initialize COM library for WIC");
		}
	}

	void suku_drawing_preinit_D2D(ComPtr<ID2D1Factory1>* ppD2DFactory)
	{
		// Init D2D resource
		ComPtr<ID2D1Factory1> localFactory;
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, localFactory.GetAddressOf());
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create D2D factory");
			if (ppD2DFactory) *ppD2DFactory = nullptr;
			return;
		}
		if (ppD2DFactory)
			*ppD2DFactory = std::move(localFactory);
	}


	void suku_drawing_postinit(HWND _hWnd)
	{
//#ifdef USE_D3D11
//		ComPtr<IDXGIDevice> dxgiDevice;
//		Graphics::pD3DDevice.As(&dxgiDevice);
//
//		D2DFactoryGlobal::getD2DFactory()->CreateDevice(dxgiDevice.Get(), &pD2DDevice);
//
//		pD2DDevice->CreateDeviceContext(
//			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
//			&pD2DContext
//		);
//#else
		RECT rc;
		GetClientRect(_hWnd, &rc);
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		ComPtr<ID2D1Factory1> factory;
		factory.Attach(pD2DFactory);
		ComPtr<ID2D1HwndRenderTarget> renderTarget;
		HRESULT hr = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				_hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
			),
			&renderTarget
		);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create render target");
			pMainRenderTarget = nullptr;
			return;
		}
		pMainRenderTarget = std::move(renderTarget);
//#endif
	}

	void suku_drawing_uninit()
	{
		CoUninitialize();
		if (pMainRenderTarget) pMainRenderTarget.Reset();
	}

	void setPaintingTransform(Transform _transform)
	{
		pMainRenderTarget->SetTransform(_transform.matrix);
	}

	void beginDrawGlobal()
	{
		pMainRenderTarget->BeginDraw();
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void endDrawGlobal()
	{
		pMainRenderTarget->EndDraw();
	}

	void clearScreen()
	{
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}
}