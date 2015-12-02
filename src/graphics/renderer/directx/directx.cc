#include "directx.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "d3dx12.h"
#include <DXGI1_4.h>
#include <iostream>
#include <wrl\client.h>

using namespace phrix::graphics::renderer::directx;
using namespace phrix::graphics::renderer;
using namespace Microsoft::WRL;

namespace {
constexpr int w = 640;
constexpr int h = 480;
constexpr int frameCount = 2;

DirectxRenderer g_renderer;



ComPtr<ID3D12Device> g_d3dDevice;
ComPtr<IDXGISwapChain3> g_swapChain;
ComPtr<ID3D12CommandQueue> g_commandQueue;
ComPtr<ID3D12RootSignature> g_rootSignature;
ComPtr<ID3D12DescriptorHeap> g_rtvHeap;
ComPtr<ID3D12Resource> g_renderTargets[frameCount];
ComPtr<ID3D12CommandAllocator> g_commandAllocator;
ComPtr<ID3D12RootSignature> g_graphicsRoot;

ComPtr<ID3DBlob> g_defaultVertexShader;
ComPtr<ID3DBlob> g_defaultPixelShader;

UINT g_rtvDescriptorHeapSize;

SDL_Window *g_window;

bool inited = false;


int frameIndex;

}

void ThrowIfFailed(HRESULT res) {
	if (FAILED(res)) {
		throw;
	}
}


Renderer* getRenderer()
{
	return &g_renderer;
}

void getHardwareAdapter(IDXGIFactory4* fact, ComPtr<IDXGIAdapter1> * ad) {
	ComPtr<IDXGIAdapter1> adapter;
	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != fact->EnumAdapters1(i, &adapter); ++i) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		std::wcout << L"Found adapter: " << desc.Description << std::endl;
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			std::cout << "Software adapter. Ignoring." << std::endl;
			continue;
		}

		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
			std::cout << "Was able to create device with required feature level." << std::endl;
			*ad = adapter;
			break;
		}
		else {
			std::cout << "Unable to create device with required feature level." << std::endl;
		}
	}
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

#if defined(_DEBUG)
	// Enable the D3D12 debug layer.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
		}


	}
#endif

	ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	getHardwareAdapter(factory.Get(), &hardwareAdapter);

	ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_d3dDevice)));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(g_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue)));

	DXGI_SWAP_CHAIN_DESC chainDesc = {};
	chainDesc.BufferCount = frameCount;
	chainDesc.BufferDesc.Width = w;
	chainDesc.BufferDesc.Height = h;
	chainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	chainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	chainDesc.OutputWindow = info.info.win.window;
	chainDesc.SampleDesc.Count = 1;
	chainDesc.Windowed = TRUE;

	ComPtr<IDXGISwapChain> swapChain;
	ThrowIfFailed(factory->CreateSwapChain(g_commandQueue.Get(), &chainDesc, &swapChain));
	ThrowIfFailed(swapChain.As(&g_swapChain));

	ThrowIfFailed(factory->MakeWindowAssociation(info.info.win.window, DXGI_MWA_NO_ALT_ENTER));
	frameIndex = g_swapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(g_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_rtvHeap)));

		g_rtvDescriptorHeapSize = g_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < frameCount; ++i)
		{
			ThrowIfFailed(g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i])));
			g_d3dDevice->CreateRenderTargetView(g_renderTargets[i].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, g_rtvDescriptorHeapSize);
		}
	}

	ThrowIfFailed(g_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator)));

	inited = true;
	return true;
}

void DirectxRenderer::present()
{
	g_swapChain->Present(0, 0);
}

void phrix::graphics::renderer::directx::DirectxRenderer::loadContent()
{
	
}

void phrix::graphics::renderer::directx::DirectxRenderer::renderScene(Scene * s)
{
}

Renderer * phrix::graphics::renderer::directx::getRenderer()
{
	return &g_renderer;
}
