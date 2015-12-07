#include "directx.h"

#include "dxresourcefactory.h"
#include "jobmanager.h"
#include "game.h"
#include "camera.h"

// Library incudes
#include "d3dx12.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include <DXGI1_4.h>
#include <iostream>
#include <wrl\client.h>
#include <algorithm>

using namespace phrix::graphics;
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
ComPtr<ID3D12CommandAllocator> g_commandAllocator[frameCount];
ComPtr<ID3D12RootSignature> g_graphicsRoot;

ComPtr<ID3DBlob> g_defaultVertexShader;
ComPtr<ID3DBlob> g_defaultPixelShader;

ComPtr<ID3D12Fence> g_fence;
HANDLE g_fenceEvent;

UINT g_rtvDescriptorHeapSize;

SDL_Window *g_window;

bool inited = false;


int g_frameIndex;
uint64_t g_fenceValue = 0;
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

	if (g_window == nullptr) {
		std::cerr << "failed to create window: " << SDL_GetError() << std::endl;
		throw;
	}

	SDL_VERSION(&info.version);

	if (SDL_GetWindowWMInfo(g_window, &info) == SDL_FALSE) {
		std::cerr << "Failed to get window info: " << SDL_GetError();
		throw;
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
	g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();

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

	ThrowIfFailed(g_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator[0])));
	ThrowIfFailed(g_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator[1])));

	g_d3dDevice->CreateFence(g_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence));
	g_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	inited = true;
	return true;
}

void DirectxRenderer::present()
{
	waitForGpuComplete();
	g_swapChain->Present(0, 0);
	g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();
}

void DirectxRenderer::loadContent()
{
	
}


class DxEndFrameJob : public phrix::game::Job {
public:
	void addCommandlist(ID3D12CommandList * cl) {
		std::lock_guard<std::mutex> l(m_listMutex);
		m_commandLists.push_back(cl);
	}

private:
	std::vector<ComPtr<ID3D12CommandList>> m_commandLists;

	void run() override
	{
		std::lock_guard<std::mutex> l(m_listMutex);
		std::unique_ptr<ID3D12CommandList*[]> submit(new ID3D12CommandList*[m_commandLists.size()]);
		for (size_t i = 0; i < m_commandLists.size(); i++)
		{
			submit[i] = m_commandLists[i].Get();
		}

		g_commandQueue->ExecuteCommandLists(m_commandLists.size(), submit.get());
		g_renderer.present();
	}

	std::mutex m_listMutex;
};

class DxRenderCameraJob : public phrix::game::Job {
public:
	DxRenderCameraJob(phrix::game::Camera * camera, DxEndFrameJob* ej) : Job(), m_camera(camera), m_endj(ej) {}
private:
	void run() override
	{
		ComPtr<ID3D12GraphicsCommandList> cmdList;
		ThrowIfFailed(g_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator[g_frameIndex].Get(), nullptr, IID_PPV_ARGS(&cmdList)));
		D3D12_CPU_DESCRIPTOR_HANDLE rtv;
		if (m_camera->target) {
			// Get the render targets CPU handle then then set it to the RTV
		}
		else {
			// get the pack buffers rtv then use it to do the rendering

			rtv = CD3DX12_CPU_DESCRIPTOR_HANDLE(g_rtvHeap->GetCPUDescriptorHandleForHeapStart(), g_frameIndex, g_rtvDescriptorHeapSize);
		}
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		cmdList->OMSetRenderTargets(1, &rtv, 0, nullptr);
		cmdList->ClearRenderTargetView(rtv, reinterpret_cast<float *>(&m_camera->clearColor), 0, nullptr);
		/* TODO:
		Iterate through each of the pipeline state objects attached to the renderables.
		Set them then call render on them.
		*/


		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(g_renderTargets[g_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		// This command list is finished, it will automatically be submitted when the end frame job has been completed.
		cmdList->Close();
		
		m_endj->addCommandlist(cmdList.Get());
	}
	phrix::game::Camera * m_camera;
	DxEndFrameJob * m_endj;
};

void DirectxRenderer::renderGame(phrix::game::Game *g)
{
	ThrowIfFailed(g_commandAllocator[g_frameIndex]->Reset());

	auto scene = g->getScene();
	auto cameras = scene->getCameras();
	auto jobManager = g->getJobManager();
	std::unique_ptr<phrix::game::Job> endj(new DxEndFrameJob());
	auto ej = static_cast<DxEndFrameJob*>(endj.get());

	if (cameras.size() > 0) {
		auto mainCamera = scene->getMainCamera();
		std::unique_ptr<phrix::game::Job> mainRenderj(new DxRenderCameraJob(mainCamera,ej));
		
		// Itterate through each of the cameras and add a job for them.
		std::for_each(cameras.begin(), cameras.end(), [jobManager, &mainRenderj, mainCamera, ej](phrix::game::Camera * c) {
			if (c != mainCamera)
			{
				std::unique_ptr<phrix::game::Job> cj(new DxRenderCameraJob(c, ej));
				// main camera should be done last.
				mainRenderj->addDependancy(cj.get());
				jobManager->sched(cj);
			}
		});

		endj->addDependancy(mainRenderj.get());
		jobManager->sched(mainRenderj);
	} 
	else {
		throw;
	}

	jobManager->sched(endj);
}

void phrix::graphics::renderer::directx::DirectxRenderer::waitForGpuComplete()
{
	// not the most effeciant
	auto waitValue = g_fenceValue;
	g_fenceValue++;
	g_commandQueue->Signal(g_fence.Get(), waitValue);

	ThrowIfFailed(g_fence->SetEventOnCompletion(waitValue, g_fenceEvent));
	WaitForSingleObject(g_fenceEvent, INFINITE);
}

std::unique_ptr<GraphicsResourceFactory> DirectxRenderer::getResourceFactory()
{
	return std::unique_ptr<GraphicsResourceFactory>(new DxGraphicsResourceFactory());
}

Renderer * phrix::graphics::renderer::directx::getRenderer()
{
	return &g_renderer;
}
