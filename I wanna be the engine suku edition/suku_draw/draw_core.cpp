#include "draw_core.h"
#include "includes.h"
#include "../framework.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	ComPtr<ID2D1HwndRenderTarget> pMainRenderTarget = nullptr;	// Render target

	void suku_drawing_preinit_WIC(ComPtr<IWICImagingFactory>* ppWICFactory)
	{
		// Init WIC resource
		HRESULT hr = CoInitialize(nullptr);
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
	}

	void suku_drawing_preinit_D2D(ComPtr<ID2D1Factory>* ppD2DFactory)
	{
		// Init D2D resource
		ComPtr<ID2D1Factory> localFactory;
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
		RECT rc;
		GetClientRect(_hWnd, &rc);
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		ComPtr<ID2D1Factory> factory;
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