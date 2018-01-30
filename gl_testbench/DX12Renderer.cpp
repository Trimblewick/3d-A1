#include "DX12Renderer.h"


DX12Renderer::DX12Renderer()
{

}

DX12Renderer::~DX12Renderer()
{
}


LRESULT CALLBACK DX12Renderer::EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//If one case is hit the code will execute everything down until a break;

	switch (message)
	{

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Material * DX12Renderer::makeMaterial(const std::string & name)
{
	return nullptr;
}

Mesh * DX12Renderer::makeMesh()
{
	return nullptr;
}

VertexBuffer * DX12Renderer::makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage)
{
	return nullptr;
}

Texture2D * DX12Renderer::makeTexture2D()
{
	return nullptr;
}

Sampler2D * DX12Renderer::makeSampler2D()
{
	return nullptr;
}

RenderState * DX12Renderer::makeRenderState()
{
	return nullptr;
}

std::string DX12Renderer::getShaderPath()
{
	return std::string();
}

std::string DX12Renderer::getShaderExtension()
{
	return std::string();
}

ConstantBuffer * DX12Renderer::makeConstantBuffer(std::string NAME, unsigned int location)
{
	return nullptr;
}

Technique * DX12Renderer::makeTechnique(Material *, RenderState *)
{
	return nullptr;
}

int DX12Renderer::initialize(unsigned int width, unsigned int height)
{
	m_pWindow = new Window(width, height, EventHandler);
	m_pD3DFactory = new D3DFactory();


	m_pCommandQueue = m_pD3DFactory->CreateCQ();

	DXGI_MODE_DESC descMode = {};
	descMode.Width = width;
	descMode.Height = height;
	descMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SAMPLE_DESC descSample = {};
	descSample.Count = 1;

	DXGI_SWAP_CHAIN_DESC descSwapChain = {};
	descSwapChain.BufferCount = g_iBackBufferCount;
	descSwapChain.BufferDesc = descMode;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	descSwapChain.OutputWindow = m_pWindow->GetWindowHandle();
	descSwapChain.SampleDesc = descSample;
	descSwapChain.Windowed = false;

	m_pSwapChain = m_pD3DFactory->CreateSwapChain(descSwapChain, m_pCommandQueue);

	m_pDHrenderTargets = m_pD3DFactory->CreateDH(g_iBackBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, false);
	int iDHIncrementSize = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHrenderTargets->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < g_iBackBufferCount; ++i)
	{
		m_pFenceValues[i] = 0;
		m_ppFenceFrame[i] = m_pD3DFactory->CreateFence();
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppRenderTargets[i]));
		m_pD3DFactory->GetDevice()->CreateRenderTargetView(m_ppRenderTargets[i], nullptr, handleDH);
		handleDH.ptr += iDHIncrementSize;
		
		m_ppCommandAllocators[i] = m_pD3DFactory->CreateCA();
		m_ppCommandLists[i] = m_pD3DFactory->CreateCL(m_ppCommandAllocators[i]);
	}

	return 0;
}

void DX12Renderer::setWinTitle(const char * title)
{
	size_t wn = mbsrtowcs(NULL, &title, 0, NULL);
	wchar_t * buf = new wchar_t[wn + 1]();
	mbsrtowcs(buf, &title, wn + 1, NULL);
	

	m_pWindow->SetTitle(buf);
	delete[] buf;
}

void DX12Renderer::present()
{


	m_pSwapChain->Present(0, 0);

}

int DX12Renderer::shutdown()
{
	return 0;
}

void DX12Renderer::setClearColor(float r, float g, float b, float a)
{
	m_pClearColor[0] = r;
	m_pClearColor[1] = g;
	m_pClearColor[2] = b;
	m_pClearColor[3] = a;
}

void DX12Renderer::clearBuffer(unsigned int flag = -1)
{
	int iFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	int iDHIncrementSize = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHrenderTargets->GetCPUDescriptorHandleForHeapStart();
	handleDH.ptr += iDHIncrementSize * iFrameIndex;

	m_ppCommandLists[iFrameIndex]->ClearRenderTargetView(handleDH, m_pClearColor, NULL, nullptr);
}

void DX12Renderer::setRenderState(RenderState * ps)
{

}

void DX12Renderer::submit(Mesh * mesh)
{
}

void DX12Renderer::frame()
{
}
