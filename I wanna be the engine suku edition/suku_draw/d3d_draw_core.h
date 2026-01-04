#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace suku
{
	namespace Graphics
	{
		using Microsoft::WRL::ComPtr;
		extern ComPtr<ID3D11Device>           pD3DDevice;
		extern ComPtr<ID3D11DeviceContext>    pD3DDeviceContext;
		extern ComPtr<IDXGISwapChain1>         pSwapChain;

		void createD3DDevice();
		void createSwapChain(HWND _hWnd);

	}
}