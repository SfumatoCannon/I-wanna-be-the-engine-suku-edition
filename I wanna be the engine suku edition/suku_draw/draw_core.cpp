#include "pch.h"
#include "draw_core.h"
#include "includes.h"
#include "../suku_foundation/includes.h"
#include "d3d_draw_core.h"

namespace suku
{
	namespace graphics
	{
		ComPtr<ID2D1Device> pD2DDevice;
		ComPtr<ID2D1DeviceContext> pD2DContext;
		ComPtr<ID2D1Bitmap1> pScreenTargetBitmap;

		void suku_wic_preinit(ComPtr<IWICImagingFactory>* ppWICFactory)
		{
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

		void suku_d2d_preinit(ComPtr<ID2D1Factory1>* ppD2DFactory)
		{
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
			suku::graphics::suku_d3d_postinit(_hWnd);

			// Create D2D Device & Device Context
			ComPtr<IDXGIDevice> dxgiDevice;
			HRESULT hr = graphics::D3DFactoryGlobal::GetD3DDevice().As(&dxgiDevice);

			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to get DXGI Device from D3D Device");
				return;
			}

			hr = D2DFactoryGlobal::getD2DFactory()->CreateDevice(dxgiDevice.Get(), &pD2DDevice);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create pD2DDevice");
				return;
			}

			hr = pD2DDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&pD2DContext
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create pD2DContext");
				return;
			}

			// Set D2D Device Context Target
			ComPtr<IDXGISurface> dxgiBackBuffer;
			hr = graphics::pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer));
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create dxgiBackBuffer");
				return;
			}
			D2D1_BITMAP_PROPERTIES1 props = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(
					DXGI_FORMAT_B8G8R8A8_UNORM,
					D2D1_ALPHA_MODE_PREMULTIPLIED
				)
			);
			hr = pD2DContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				&props,
				&pScreenTargetBitmap
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create pScreenTargetBitmap");
				return;
			}
			pD2DContext->SetTarget(pScreenTargetBitmap.Get());
		}

		void suku_drawing_uninit()
		{
			if (pD2DContext)
			{
				pD2DContext->SetTarget(nullptr);
				pD2DContext.Reset();
			}
			if (pD2DDevice) pD2DDevice.Reset();
		}

		void setPaintingTransform(Transform _transform)
		{
			pD2DContext->SetTransform(_transform.matrix);
		}

		ComPtr<ID2D1SolidColorBrush> createSolidColorBrush(Color _color)
		{
			ComPtr<ID2D1SolidColorBrush> brush;
			D2D1_COLOR_F d2dColor = D2D1::ColorF(
				_color.r() / 255.0f,
				_color.g() / 255.0f,
				_color.b() / 255.0f,
				_color.alpha
			);
			HRESULT hr = pD2DContext->CreateSolidColorBrush(
				d2dColor,
				&brush
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create solid color brush");
				return nullptr;
			}
			return brush;
		}

		void drawGeometryOutline(const ComPtr<ID2D1Geometry>& _geometry, const ComPtr<ID2D1Brush>& _brush, float _strokeWidth, const ComPtr<ID2D1StrokeStyle>& _strokeStyle)
		{
			pD2DContext->DrawGeometry(
				_geometry.Get(),
				_brush.Get(),
				_strokeWidth,
				_strokeStyle.Get()
			);
		}

		void drawGeometryFill(const ComPtr<ID2D1Geometry>& _geometry, const ComPtr<ID2D1Brush>& _brush)
		{
			pD2DContext->FillGeometry(
				_geometry.Get(),
				_brush.Get()
			);
		}

		void drawBitmap(const ComPtr<ID2D1Bitmap1>& _bitmap, float _opacity)
		{
			pD2DContext->DrawBitmap(
				_bitmap.Get(),
				nullptr,
				_opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				nullptr
			);
		}

		void drawBitmap(const ComPtr<ID2D1Bitmap1>& _bitmap, const D2D1_RECT_F& _destRect, const D2D1_RECT_F& _srcRect, float _opacity)
		{
			pD2DContext->DrawBitmap(
				_bitmap.Get(),
				&_destRect,
				_opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				&_srcRect
			);
		}

		ComPtr<ID2D1Bitmap1> createBitmapFromWicBitmap(const ComPtr<IWICBitmap>& _wicBitmap)
		{
			ComPtr<ID2D1Bitmap1> bitmap;
			D2D1_BITMAP_PROPERTIES1 props = {};
			props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
			pD2DContext->CreateBitmapFromWicBitmap(
				_wicBitmap.Get(),
				&props,
				&bitmap
			);
			return bitmap;
		}

		ComPtr<ID2D1Bitmap1> createBitmap(UINT _width, UINT _height)
		{
			D2D1_BITMAP_PROPERTIES1 props = {};
			props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

			props.bitmapOptions = D2D1_BITMAP_OPTIONS_NONE;

			ComPtr<ID2D1Bitmap1> newBitmap;

			HRESULT hr = pD2DContext->CreateBitmap(
				D2D1::SizeU(_width, _height),
				nullptr,
				0,
				&props,
				&newBitmap
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create bitmap");
			}
			return newBitmap;
		}

		ComPtr<ID2D1Bitmap1> createLayerBitmap(UINT _width, UINT _height)
		{
			D2D1_BITMAP_PROPERTIES1 props = {};
			props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
			props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

			props.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

			ComPtr<ID2D1Bitmap1> newLayerBitmap;

			HRESULT hr = pD2DContext->CreateBitmap(
				D2D1::SizeU(_width, _height),
				nullptr,
				0,
				&props,
				&newLayerBitmap
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create layer bitmap");
			}
			return newLayerBitmap;
		}

		void beginDrawGlobal()
		{
			pD2DContext->SetTarget(pScreenTargetBitmap.Get());
			pD2DContext->BeginDraw();
			pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}

		void endDrawGlobal()
		{
			pD2DContext->EndDraw();
			pD2DContext->SetTarget(nullptr);
			graphics::pSwapChain->Present(1, 0);
		}

		void clearScreen()
		{
			pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}
	}
}