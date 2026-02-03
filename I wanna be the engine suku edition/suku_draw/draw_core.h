#pragma once
#include <wrl/client.h>
#include "d3d_draw_core.h"
#include <d2d1.h>
#include "color.h"
#include "transform.h"
#include "scale_mode.h"

namespace suku
{
	namespace graphics
	{
		using Microsoft::WRL::ComPtr;

		extern ComPtr<ID2D1Device> pD2DDevice;
		extern ComPtr<ID2D1DeviceContext> pD2DContext;
		extern ComPtr<ID2D1Bitmap1> pScreenTargetBitmap;

		void suku_wic_preinit(ComPtr<IWICImagingFactory>* ppWICFactory);
		void suku_d2d_preinit(ComPtr<ID2D1Factory1>* ppD2DFactory);
		void suku_drawing_postinit(HWND _hWnd);
		void suku_drawing_uninit();

		void setPaintingTransform(Transform _transform);
		ComPtr<ID2D1SolidColorBrush> createSolidColorBrush(Color _color);
		void drawGeometryOutline(
			const ComPtr<ID2D1Geometry>& _geometry,
			const ComPtr<ID2D1Brush>& _brush,
			float _strokeWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& _strokeStyle = nullptr
		);
		void drawGeometryFill(
			const ComPtr<ID2D1Geometry>& _geometry,
			const ComPtr<ID2D1Brush>& _brush
		);
		void drawBitmap(
			const ComPtr<ID2D1Bitmap1>& _bitmap,
			float _opacity = 1.0,
			ScaleMode _scaleMode = ScaleMode::Linear
		);
		void drawBitmap(
			const ComPtr<ID2D1Bitmap1>& _bitmap,
			const D2D1_RECT_F& _destRect,
			const D2D1_RECT_F& _srcRect,
			float _opacity = 1.0,
			ScaleMode _scaleMode = ScaleMode::Linear
		);
		void drawEffect(
			const ComPtr<ID2D1Effect>& _effect,
			ScaleMode _scaleMode = ScaleMode::Linear
		);
		void drawEffect(
			const ComPtr<ID2D1Effect>& _effect,
			const D2D1_RECT_F& _destRect,
			const D2D1_RECT_F& _srcRect,
			ScaleMode _scaleMode = ScaleMode::Linear
		);
		
		ComPtr<ID2D1Bitmap1> createBitmapFromWicBitmap(const ComPtr<IWICBitmap>& _wicBitmap);
		ComPtr<ID2D1Bitmap1> createBitmap(UINT _width, UINT _height);
		ComPtr<ID2D1Bitmap1> createLayerBitmap(UINT _width, UINT _height);

		void beginDrawGlobal();
		void endDrawGlobal();
		void clearScreen();

		class WICFactoryGlobal
		{
		public:
			static IWICImagingFactory* getWICFactory()
			{
				static WICFactoryGlobal instance;
				return instance.pWICFactory_.Get();
			}
			WICFactoryGlobal(const WICFactoryGlobal&) = delete;
			WICFactoryGlobal& operator=(const WICFactoryGlobal&) = delete;
		private:
			WICFactoryGlobal()
			{
				suku_wic_preinit(&pWICFactory_);
			}
			ComPtr<IWICImagingFactory> pWICFactory_ = nullptr;
		};

		class D2DFactoryGlobal
		{
		public:
			static ID2D1Factory1* getD2DFactory()
			{
				static D2DFactoryGlobal instance;
				return instance.pD2DFactory_.Get();
			}
			D2DFactoryGlobal(const D2DFactoryGlobal&) = delete;
			D2DFactoryGlobal& operator=(const D2DFactoryGlobal&) = delete;
		private:
			D2DFactoryGlobal()
			{
				suku_d2d_preinit(&pD2DFactory_);
			}
			ComPtr<ID2D1Factory1> pD2DFactory_ = nullptr;
		};
	}
}