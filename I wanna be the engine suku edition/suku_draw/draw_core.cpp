#include "draw_core.h"
#include "includes.h"
#include "../framework.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	ID2D1Factory* pD2DFactory = nullptr;	// Direct2D factory
	ID2D1HwndRenderTarget* pMainRenderTarget = nullptr;	// Render target
	IWICImagingFactory* pIWICFactory;

	void suku_drawing_preinit()
	{
		// Init WIC resource
		HRESULT hr = CoInitialize(nullptr);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pIWICFactory)
			);
		}
	}

	void suku_drawing_postinit(HWND _hWnd)
	{
		// Init D2D resource
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
		if (FAILED(hr))
		{
			ERRORWINDOW_GLOBAL("Failed to create D2D factory");
			return;
		}

		RECT rc;
		GetClientRect(_hWnd, &rc);

		hr = pD2DFactory->CreateHwndRenderTarget(
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
			return;
		}
	}

	void suku_drawing_uninit()
	{
		CoUninitialize();
		release_safe(pMainRenderTarget);
		release_safe(pD2DFactory);
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