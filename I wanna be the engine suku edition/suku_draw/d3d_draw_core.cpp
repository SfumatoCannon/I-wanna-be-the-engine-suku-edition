#include "pch.h"
#include "d3d_draw_core.h"

namespace suku
{
	namespace Graphics
	{
		ComPtr<IDXGISwapChain1>         pSwapChain;

		void createD3DDevice(ComPtr<ID3D11Device>& _pD3DDevice,
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext)
		{
			D3D_FEATURE_LEVEL levels[] = {
				D3D_FEATURE_LEVEL_11_0,
			};

			D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				D3D11_CREATE_DEVICE_BGRA_SUPPORT,
				levels,
				_countof(levels),
				D3D11_SDK_VERSION,
				_pD3DDevice.GetAddressOf(),
				nullptr,
				_pD3DDeviceContext.GetAddressOf()
			);
		}

		void createSwapChain(HWND _hWnd)
		{
			ComPtr<IDXGIDevice> dxgiDevice;
			const auto& pD3DDevice = Graphics::D3DFactoryGlobal::GetD3DDevice();
			pD3DDevice.As(&dxgiDevice);

			ComPtr<IDXGIAdapter> adapter;
			dxgiDevice->GetAdapter(&adapter);

			ComPtr<IDXGIFactory2> factory;
			adapter->GetParent(IID_PPV_ARGS(&factory));

			RECT rc;
			GetClientRect(_hWnd, &rc);

			float dpi = GetDpiForWindow(_hWnd);
			float scale = dpi / 96.0f;

			UINT pixelWidth = UINT((rc.right - rc.left) * scale);
			UINT pixelHeight = UINT((rc.bottom - rc.top) * scale);

			DXGI_SWAP_CHAIN_DESC1 desc = {};
			desc.Width = pixelWidth;
			desc.Height = pixelHeight;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.BufferCount = 2;
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

			factory->CreateSwapChainForHwnd(
				pD3DDevice.Get(),
				_hWnd,
				&desc,
				nullptr,
				nullptr,
				&pSwapChain
			);
		}

		void suku_d3d_preinit(ComPtr<ID3D11Device>& _pD3DDevice,
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext)
		{
			createD3DDevice(_pD3DDevice, _pD3DDeviceContext);
		}

		void suku_d3d_postinit(HWND _hWnd)
		{
			createSwapChain(_hWnd);
		}

		void suku_d3d_uninit(ComPtr<ID3D11Device>& _pD3DDevice,
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext)
		{
			if (_pD3DDeviceContext)
			{
				_pD3DDeviceContext->ClearState();
				_pD3DDeviceContext->Flush();
			}

			pSwapChain.Reset();

			_pD3DDeviceContext.Reset();

			_pD3DDevice.Reset();
		}
	}
}