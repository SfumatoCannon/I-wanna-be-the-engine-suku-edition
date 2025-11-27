#pragma once
#include "../framework.h"


namespace suku
{
	extern ID2D1Factory* pD2DFactory;
	extern ID2D1HwndRenderTarget* pMainRenderTarget;
	class Transform;

	template<typename T>
	void addRef_safe(T* pCom) { if (pCom) pCom->AddRef(); }
	template<typename T>
	void release_safe(T* pCom) { if (pCom) { pCom->Release(); pCom = nullptr; } }

	void suku_drawing_preinit(IWICImagingFactory** ppWICFactory);
	void suku_drawing_postinit(HWND _hWnd);
	void suku_drawing_uninit();

	void setPaintingTransform(Transform _transform);

	void beginDraw(HWND hWnd);
	void endDraw();
	void clearScreen();

	class WICFactoryGlobal
	{
	private:
		IWICImagingFactory* pWICFactory_ = nullptr;
	public:
		static IWICImagingFactory* getWICFactory()
		{
			static WICFactoryGlobal instance;
			return instance.pWICFactory_;
		}
		WICFactoryGlobal()
		{
			suku_drawing_preinit(&pWICFactory_);
		}
		WICFactoryGlobal(const WICFactoryGlobal&) = delete;
		WICFactoryGlobal& operator=(const WICFactoryGlobal&) = delete;
	};
}