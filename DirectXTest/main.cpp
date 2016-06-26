#include <Windows.h>
#include "DXCore.h"

HRESULT InitWindow(HINSTANCE instance, int cmdshow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND mainWnd;

int WINAPI
WinMain(HINSTANCE instance, HINSTANCE previnst, LPSTR cmdline, int cmdshow)
{
	UNREFERENCED_PARAMETER(previnst);
	UNREFERENCED_PARAMETER(cmdline);

	if (FAILED(InitWindow(instance, cmdshow))) {
		return 0;
	}

	DXCore *dxc = new DXCore(mainWnd);
	if (dxc == NULL) {
		return 0;
	}

	if (FAILED(dxc->InitDevice())) {
		return 0;
	}

	MSG msg = {0};

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			dxc->Render();
		}
	}

	dxc->CleanupDevice();

	return (int)(msg.wParam);
}

HRESULT
InitWindow(HINSTANCE instance, int cmdshow)
{
	WNDCLASSEX wc;

	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(instance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("DIRECTX11TEST");
	wc.hIconSm = NULL;

	if (!RegisterClassEx(&wc)) {
		return E_FAIL;
	}

	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	mainWnd = CreateWindow(TEXT("DIRECTX11TEST"), TEXT("DirectX11 Test App"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, (rc.right - rc.left), (rc.bottom - rc.top), NULL, NULL, instance, NULL);
	if (mainWnd == NULL) {
		return E_FAIL;
	}

	ShowWindow(mainWnd, cmdshow);

	return S_OK;
}

LRESULT CALLBACK
WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(wnd, &ps);
		EndPaint(wnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(wnd, msg, wp, lp);
	}
	return 0;
}
