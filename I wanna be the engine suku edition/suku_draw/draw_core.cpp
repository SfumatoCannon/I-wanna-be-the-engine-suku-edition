#include "draw_core.h"
#include "includes.h"
#include "../framework.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	ID2D1HwndRenderTarget* pMainRenderTarget = nullptr;	// Render target

	void suku_drawing_preinit_WIC(IWICImagingFactory** ppWICFactory)
	{
		// Init WIC resource
		HRESULT hr = CoInitialize(nullptr);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(ppWICFactory)
			);
		}
	}

	void suku_drawing_preinit_D2D(ID2D1Factory** ppD2DFactory)
	{
		// Init D2D resource
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, ppD2DFactory);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create D2D factory");
			*ppD2DFactory = nullptr;
			return;
		}
	}

	void suku_drawing_postinit(HWND _hWnd)
	{
		RECT rc;
		GetClientRect(_hWnd, &rc);
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				_hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
			),
			&pMainRenderTarget
		);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create render target");
			pMainRenderTarget = nullptr;
			return;
		}
	}

	void suku_drawing_uninit()
	{
		CoUninitialize();
		release_safe(pMainRenderTarget);
	}

	void setPaintingTransform(Transform _transform)
	{
		pMainRenderTarget->SetTransform(_transform.matrix);
	}

	void beginDraw(HWND hWnd)
	{
		pMainRenderTarget->BeginDraw();
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void endDraw()
	{
		pMainRenderTarget->EndDraw();
	}

	void clearScreen()
	{
		pMainRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}
}