#pragma once
#include "../framework.h"


namespace suku
{
	extern ID2D1HwndRenderTarget* pMainRenderTarget;
	class Transform;

	template<typename T>
	void addRef_safe(T* pCom) { if (pCom) pCom->AddRef(); }
	template<typename T>
	void release_safe(T* pCom) { if (pCom) { pCom->Release(); pCom = nullptr; } }

	void suku_drawing_preinit_WIC(IWICImagingFactory** ppWICFactory);
	void suku_drawing_preinit_D2D(ID2D1Factory** ppD2DFactory);
	void suku_drawing_postinit(HWND _hWnd);
	void suku_drawing_uninit();

	void setPaintingTransform(Transform _transform);

	void beginDraw(HWND hWnd);
	void endDraw();
	void clearScreen();

	class WICFactoryGlobal
	{
	public:
		static IWICImagingFactory* getWICFactory()
		{
			static WICFactoryGlobal instance;
			return instance.pWICFactory_;
		}
		WICFactoryGlobal(const WICFactoryGlobal&) = delete;
		WICFactoryGlobal& operator=(const WICFactoryGlobal&) = delete;
	private:
		WICFactoryGlobal()
		{
			suku_drawing_preinit_WIC(&pWICFactory_);
		}
		IWICImagingFactory* pWICFactory_ = nullptr;
	};

	class D2DFactoryGlobal
	{
	public:
		static ID2D1Factory* getD2DFactory()
		{
			static D2DFactoryGlobal instance;
			return instance.pD2DFactory_;
		}
		D2DFactoryGlobal(const D2DFactoryGlobal&) = delete;
		D2DFactoryGlobal& operator=(const D2DFactoryGlobal&) = delete;
	private:
		D2DFactoryGlobal()
		{
			suku_drawing_preinit_D2D(&pD2DFactory_);
		}
		ID2D1Factory* pD2DFactory_ = nullptr;
	};
}