#pragma once
#include "../framework.h"


namespace suku
{
	extern ID2D1Factory* pD2DFactory;
	extern ID2D1HwndRenderTarget* pMainRenderTarget;
	extern IWICImagingFactory* pIWICFactory;
	class Transform;

	template<typename T>
	void addRef_safe(T* pCom) { if (pCom) pCom->AddRef(); }
	template<typename T>
	void release_safe(T* pCom) { if (pCom) { pCom->Release(); pCom = nullptr; } }

	void suku_drawing_preinit();
	void suku_drawing_postinit(HWND _hWnd);
	void suku_drawing_uninit();

	void setPaintingTransform(Transform _transform);

	void beginDraw(HWND hWnd);
	void endDraw();
	void clearScreen();
}