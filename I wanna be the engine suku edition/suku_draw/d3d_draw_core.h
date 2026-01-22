#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace suku
{
	namespace Graphics
	{
		using Microsoft::WRL::ComPtr;

		extern ComPtr<IDXGISwapChain1>         pSwapChain;

		void createD3DDevice(ComPtr<ID3D11Device>& _pD3DDevice,
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext);
		void createSwapChain(HWND _hWnd);

		void suku_d3d_preinit(ComPtr<ID3D11Device>& _pD3DDevice, 
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext);
		void suku_d3d_postinit(HWND _hWnd);
		void suku_d3d_uninit(ComPtr<ID3D11Device>& _pD3DDevice,
			ComPtr<ID3D11DeviceContext>& _pD3DDeviceContext);

		class D3DFactoryGlobal
		{
		public:
			static ComPtr<ID3D11Device> GetD3DDevice()
			{
				return GetInstanceStruct().first;
			}
			static ComPtr<ID3D11DeviceContext> GetD3DDeviceContext()
			{
				return GetInstanceStruct().second;
			}
			D3DFactoryGlobal(const D3DFactoryGlobal&) = delete;
			D3DFactoryGlobal& operator=(const D3DFactoryGlobal&) = delete;
		private:
			static std::pair<ComPtr<ID3D11Device>, ComPtr<ID3D11DeviceContext>> GetInstanceStruct()
			{
				static D3DFactoryGlobal instance;
				return { instance.pD3DDevice_, instance.pD3DDeviceContext_ };
			}
			D3DFactoryGlobal()
			{
				suku_d3d_preinit(pD3DDevice_, pD3DDeviceContext_);
			}
			~D3DFactoryGlobal()
			{
				suku_d3d_uninit(pD3DDevice_, pD3DDeviceContext_);
			}
			ComPtr<ID3D11Device> pD3DDevice_ = nullptr;
			ComPtr<ID3D11DeviceContext> pD3DDeviceContext_ = nullptr;
		};
	}
}