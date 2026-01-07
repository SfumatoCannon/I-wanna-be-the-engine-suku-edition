#pragma once
#include <wrl/client.h>
#include "d3d_draw_core.h"


#define USE_D3D11

namespace suku
{
    using Microsoft::WRL::ComPtr;

//#ifdef USE_D3D11
//	extern ComPtr<ID2D1Device> pD2DDevice;
//	extern ComPtr<ID2D1DeviceContext> pD2DContext;
//#else
	extern ComPtr<ID2D1HwndRenderTarget> pMainRenderTarget;
//#endif
	class Transform;

	template<typename T>
	void addRef_safe(T* pCom) { if (pCom) pCom->AddRef(); }
	template<typename T>
	void release_safe(T* pCom) { if (pCom) { pCom->Release(); pCom = nullptr; } }

	// overloads for ComPtr
	template<typename T>
	void release_safe(ComPtr<T>& pCom) { if (pCom) pCom.Reset(); }
	template<typename T>
	void addRef_safe(ComPtr<T>& pCom) { if (pCom) pCom->AddRef(); }

	void suku_drawing_preinit_WIC(ComPtr<IWICImagingFactory>* ppWICFactory);
	void suku_drawing_preinit_D2D(ComPtr<ID2D1Factory1>* ppD2DFactory);
	void suku_drawing_postinit(HWND _hWnd);
	void suku_drawing_uninit();

	void setPaintingTransform(Transform _transform);

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
			suku_drawing_preinit_WIC(&pWICFactory_);
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
//#ifdef USE_D3D11
//			Graphics::suku_d3d_preinit();
//#endif
			suku_drawing_preinit_D2D(&pD2DFactory_);
		}
		ComPtr<ID2D1Factory1> pD2DFactory_ = nullptr;
	};
}