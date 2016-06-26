#include "DXCore.h"


DXCore::DXCore(void)
{
	this->wnd = NULL;
	this->driver = D3D_DRIVER_TYPE_NULL;
	this->feature = D3D_FEATURE_LEVEL_11_0;
	this->device = NULL;
	this->context = NULL;
	this->swapchain = NULL;
	this->targetview = NULL;
}

DXCore::DXCore(HWND wnd)
{
	DXCore();
	this->wnd = wnd;
}

DXCore::~DXCore(void)
{
	CleanupDevice();
}


HRESULT
DXCore::InitDevice(void)
{
	HRESULT ret = S_OK;

	UINT flag = 0;
#ifdef _DEBUG
	flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE drivertype[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT drivernum = ARRAYSIZE(drivertype);

	D3D_FEATURE_LEVEL level[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT levelnum = ARRAYSIZE(level);

	RECT rect;
	GetClientRect(this->wnd, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = this->wnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	for (int index = 0; index < drivernum; index++) {
		this->driver = drivertype[index];

		ret = D3D11CreateDeviceAndSwapChain(NULL, this->driver, NULL, flag, level, levelnum, D3D11_SDK_VERSION, &desc, &this->swapchain, &this->device, &this->feature, &this->context);
		if (SUCCEEDED(ret)) {
			break;
		}
	}
	if (FAILED(ret)) {
		return ret;
	}

	ID3D11Texture2D *buffer = NULL;
	ret = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&buffer);
	if (FAILED(ret)) {
		return ret;
	}
	ret = this->device->CreateRenderTargetView(buffer, NULL, &this->targetview);
	buffer->Release();
	if (FAILED(ret)) {
		return ret;
	}

	this->context->OMSetRenderTargets(1, &this->targetview, NULL);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->context->RSSetViewports(1, &vp);

	return ret;
}

void
DXCore::CleanupDevice(void)
{
	if (this->context != NULL) {
		this->context->ClearState();
	}
	if (this->targetview != NULL) {
		this->targetview->Release();
		this->targetview = NULL;
	}
	if (this->swapchain != NULL) {
		this->swapchain->Release();
		this->swapchain = NULL;
	}
	if (this->context != NULL) {
		this->context->Release();
		this->context = NULL;
	}
	if (this->device != NULL) {
		this->device->Release();
		this->device = NULL;
	}
}

void
DXCore::Render()
{
	float color[4] = {0.0f, 0.125f, 0.3f, 1.0f};
	this->context->ClearRenderTargetView(this->targetview, color);
	this->swapchain->Present(0, 0);
}
