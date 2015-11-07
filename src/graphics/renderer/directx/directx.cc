#include "directx.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include <d3d11.h>
#include <iostream>

using namespace phrix::graphics::renderer::directx;
using namespace phrix::graphics::renderer;

namespace {
DirectxRenderer g_renderer;

IDXGISwapChain *g_pSwapChain;
ID3D11Device *g_pd3dDevice;
ID3D11DeviceContext *g_pImmediateContext;
ID3D11Texture2D *g_pBackBuffer;
ID3D11RenderTargetView *g_pmainRenderTarget;

SDL_Window *g_window;

bool inited = false;

constexpr int w = 640;
constexpr int h = 480;

}


Renderer* getRenderer()
{
	return &g_renderer;
}

bool DirectxRenderer::init()
{
	if (inited)
	{
		std::cerr << "already initied!";
		return false;
	}

	g_window = SDL_CreateWindow("phrix", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, 0);
	SDL_SysWMinfo info;

	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(g_window, &info) == SDL_FALSE) {
		std::cerr << "Failed to get window info: " << SDL_GetError();
	}

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = w;
	sd.BufferDesc.Height = h;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = info.info.win.window;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;


	D3D_FEATURE_LEVEL featureLvls[] = { D3D_FEATURE_LEVEL_11_0 };

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLvls, 1,
		D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext)))
	{
		return FALSE;
	}

	if (FAILED(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&g_pBackBuffer))) {
		return FALSE;
	}

	auto hr = g_pd3dDevice->CreateRenderTargetView(g_pBackBuffer, NULL, &g_pmainRenderTarget);
	g_pBackBuffer->Release();

	if (FAILED(hr))
		return FALSE;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pmainRenderTarget, NULL);

	inited = true;
	return true;
}

std::unique_ptr<RendererContext> DirectxRenderer::getContext(RendererContextType)
{
	return std::unique_ptr<RendererContext>();
}

void DirectxRenderer::present()
{
	g_pSwapChain->Present(0, 0);
}

void phrix::graphics::renderer::directx::DirectxRenderer::clear(Color c)
{
	g_pImmediateContext->ClearRenderTargetView(g_pmainRenderTarget, (float*)&c);
}

Renderer * phrix::graphics::renderer::directx::getRenderer()
{
	return &g_renderer;
}
