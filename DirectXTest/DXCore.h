#pragma once

#include <windows.h>
#include <D3D11.h>
#include <D3DX11.h>


class DXCore
{
private:
	HWND wnd;

	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain *swapchain;
	ID3D11RenderTargetView *targetview;
	D3D_DRIVER_TYPE driver;
	D3D_FEATURE_LEVEL feature;

public:
	DXCore();
	DXCore(HWND);
	~DXCore(void);

	HRESULT InitDevice(void);
	void CleanupDevice(void);

	void Render(void);
};

