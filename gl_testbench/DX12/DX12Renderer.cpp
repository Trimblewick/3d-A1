#include "DX12Renderer.h"
#include "MaterialDX12.h"
#include "Sampler2DDX12.h"
#include "MeshDX12.h"
#include "../Technique.h"
#include "ResourceBindingDX12.h"
#include "RenderStateDX12.h"
#include "VertexBufferDX12.h"
#include "ConstantBufferDX12.h"
#include "Texture2DDX12.h"

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
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Material * DX12Renderer::makeMaterial(const std::string & name)
{
	return new MaterialDX12();
}

Mesh * DX12Renderer::makeMesh()
{
	return new MeshDX12();
}

VertexBuffer * DX12Renderer::makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage)
{
	return new VertexBufferDX12(size, usage, m_pD3DFactory);
}

Texture2D * DX12Renderer::makeTexture2D()
{
	return new Texture2DDX12(m_pD3DFactory, m_pDHTexture);
}

Sampler2D * DX12Renderer::makeSampler2D()
{
	return new Sampler2DDX12();
}

RenderState * DX12Renderer::makeRenderState()
{
	return nullptr;
}

std::string DX12Renderer::getShaderPath()
{
	return std::string("..\\gl_testbench\\");
}

std::string DX12Renderer::getShaderExtension()
{
	return std::string(".hlsl");
}


ConstantBuffer * DX12Renderer::makeConstantBuffer(std::string NAME, unsigned int location)
{
	return new ConstantBufferDX12(m_pD3DFactory);
}

Technique * DX12Renderer::makeTechnique(Material* m, RenderState* r)
{
	return new Technique(m, r);
}

int DX12Renderer::initialize(unsigned int width, unsigned int height)
{
	m_pWindow = new Window(width, height, EventHandler);
	m_pD3DFactory = new D3DFactory();

	m_pCommandQueue = m_pD3DFactory->CreateCQ();
	m_handleFence = CreateEvent(NULL, NULL, NULL, NULL);

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
	descSwapChain.Windowed = true;

	m_pSwapChain = m_pD3DFactory->CreateSwapChain(descSwapChain, m_pCommandQueue);

	m_pDHrenderTargets = m_pD3DFactory->CreateDH(g_iBackBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, false);
	int iDHIncrementSize = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHrenderTargets->GetCPUDescriptorHandleForHeapStart();

	//Set up for backbuffering
	for (int i = 0; i < g_iBackBufferCount; ++i)
	{
		m_pFenceValues[i] = 0;
		m_ppFenceFrame[i] = m_pD3DFactory->CreateFence();
		m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppRenderTargets[i]));

		m_pD3DFactory->GetDevice()->CreateRenderTargetView(m_ppRenderTargets[i], nullptr, handleDH);
		handleDH.ptr += iDHIncrementSize;

		m_ppCommandAllocators[i] = m_pD3DFactory->CreateCA();
		m_ppCommandLists[i] = m_pD3DFactory->CreateCL(m_ppCommandAllocators[i]);
		m_ppCommandLists[i]->Close();
	}



	//compile shaders
	std::string s = getShaderPath() + "VertexShader" + getShaderExtension();
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR vertexShaderPath = stemp.c_str();

	ID3DBlob* pVSblob = m_pD3DFactory->CompileShader(vertexShaderPath, "main", "vs_5_1");


	s = getShaderPath() + "FragmentShader" + getShaderExtension();
	stemp = std::wstring(s.begin(), s.end());
	LPCWSTR fragmentShaderPath = stemp.c_str();

	ID3DBlob* pPSblob = m_pD3DFactory->CompileShader(fragmentShaderPath, "main", "ps_5_1");


	//create root signature
	D3D12_DESCRIPTOR_RANGE tableRange[1];
	tableRange[0].NumDescriptors = 1;
	tableRange[0].BaseShaderRegister = 0;
	tableRange[0].RegisterSpace = 0;
	tableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	tableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;


	D3D12_ROOT_DESCRIPTOR_TABLE rdt;
	rdt.NumDescriptorRanges = 1;
	rdt.pDescriptorRanges = tableRange;
	

	D3D12_ROOT_PARAMETER rp[5];
	rp[0].DescriptorTable = rdt;
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	

	D3D12_STATIC_SAMPLER_DESC descStandardSampler[1];

	descStandardSampler[0].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	descStandardSampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	descStandardSampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	descStandardSampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	descStandardSampler[0].MipLODBias = 0;
	descStandardSampler[0].MaxAnisotropy = 1;
	descStandardSampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	descStandardSampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	descStandardSampler[0].MinLOD = 0.0f;
	descStandardSampler[0].MaxLOD = D3D12_FLOAT32_MAX;
	descStandardSampler[0].ShaderRegister = 0;
	descStandardSampler[0].RegisterSpace = 0;
	descStandardSampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	

	D3D12_ROOT_CONSTANTS colorConstant;
	colorConstant.Num32BitValues = 4;
	colorConstant.RegisterSpace = 0;
	colorConstant.ShaderRegister = 0;

	rp[1].Constants = colorConstant;
	rp[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rp[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	D3D12_ROOT_DESCRIPTOR rd;
	rd.RegisterSpace = 0;
	rd.ShaderRegister = 1;

	rp[2].Descriptor = rd;
	rp[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;

	rd.ShaderRegister = 2;
	rp[3].Descriptor = rd;
	rp[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;

	rd.ShaderRegister = 3;
	rp[4].Descriptor = rd;
	rp[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rp[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;


	D3D12_ROOT_SIGNATURE_DESC descRS = {};// CD3DX12_ROOT_SIGNATURE_DESC(D3D12_DEFAULT);// {};
	//descRS.Init(1, rp, 1, descStandardSampler);
	descRS.Flags =  
		D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	descRS.NumParameters = _countof(rp);// rootParameters.size();
	descRS.pParameters = rp;// rootParameters.data();
	descRS.NumStaticSamplers = 1;
	descRS.pStaticSamplers = descStandardSampler;
	

	m_pRS = m_pD3DFactory->CreateRS(&descRS);

	DXGI_SWAP_CHAIN_DESC tempSwapChainDesc;
	m_pSwapChain->GetDesc(&tempSwapChainDesc);


	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO = {};
	descPSO.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//descPSO.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	descPSO.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	descPSO.NumRenderTargets = 1;
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	descPSO.VS = { pVSblob->GetBufferPointer(), pVSblob->GetBufferSize() };
	descPSO.PS = { pPSblob->GetBufferPointer(), pPSblob->GetBufferSize() };
	descPSO.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	descPSO.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	descPSO.SampleDesc = descSample;
	descPSO.SampleMask = 0xffffffff;
	descPSO.pRootSignature = m_pRS;

	
	m_pPSOs.push_back(m_pD3DFactory->CreatePSO(&descPSO));

	descPSO.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

	m_pPSOs.push_back(m_pD3DFactory->CreatePSO(&descPSO));

	SAFE_RELEASE(pVSblob);
	SAFE_RELEASE(pPSblob);

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_rectScissor.left = (long)0;
	m_rectScissor.top = (long)0;
	m_rectScissor.right = (long)width;
	m_rectScissor.bottom = (long)height;

	m_pDHTexture = m_pD3DFactory->CreateDH(1, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

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
	int iFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	m_ppCommandLists[iFrameIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_ppRenderTargets[iFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	m_ppCommandLists[iFrameIndex]->Close();

	ID3D12CommandList* ppCLs[] = { m_ppCommandLists[iFrameIndex] };
	m_pCommandQueue->ExecuteCommandLists(1, ppCLs);
	m_pCommandQueue->Signal(m_ppFenceFrame[iFrameIndex], m_pFenceValues[iFrameIndex]);

	m_pSwapChain->Present(0, 0);

	iFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	if (m_ppFenceFrame[iFrameIndex]->GetCompletedValue() < m_pFenceValues[iFrameIndex])
	{
		m_ppFenceFrame[iFrameIndex]->SetEventOnCompletion(m_pFenceValues[iFrameIndex], m_handleFence);
		WaitForSingleObject(m_handleFence, INFINITE);
	}
	m_pFenceValues[iFrameIndex]++;
}

int DX12Renderer::shutdown()
{
	for (int i = 0; i < g_iBackBufferCount; ++i)
	{
		m_pFenceValues[i]++;											
		m_pCommandQueue->Signal(m_ppFenceFrame[i], m_pFenceValues[i]);

		m_ppFenceFrame[i]->SetEventOnCompletion(m_pFenceValues[i], m_handleFence);
		WaitForSingleObject(m_handleFence, INFINITE);
	}

	for (int i = 0; i < g_iBackBufferCount; ++i)
	{
		SAFE_RELEASE(m_ppCommandAllocators[i]);
		SAFE_RELEASE(m_ppCommandLists[i]);
		SAFE_RELEASE(m_ppFenceFrame[i]);
		SAFE_RELEASE(m_ppRenderTargets[i]);
		
	}

	for (ID3D12PipelineState* pPSO : m_pPSOs)
	{
		SAFE_RELEASE(pPSO);
	}

	SAFE_RELEASE(m_pRS);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pCommandQueue);

	if (m_pD3DFactory)
	{
		delete m_pD3DFactory;
		m_pD3DFactory = nullptr;
	}
	if (m_pWindow)
	{
		delete m_pWindow;
		m_pWindow = nullptr;
	}
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

	HRESULT hr = m_ppCommandAllocators[iFrameIndex]->Reset();
	hr = m_ppCommandLists[iFrameIndex]->Reset(m_ppCommandAllocators[iFrameIndex], nullptr);
	
	m_ppCommandLists[iFrameIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_ppRenderTargets[iFrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	int iIncrementSizeRTV = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHrenderTargets->GetCPUDescriptorHandleForHeapStart();
	handleDH.ptr += iIncrementSizeRTV * iFrameIndex;

	
	m_ppCommandLists[iFrameIndex]->ClearRenderTargetView(handleDH, m_pClearColor, NULL, nullptr);
	m_ppCommandLists[iFrameIndex]->OMSetRenderTargets(1, &handleDH, NULL, nullptr);
}

void DX12Renderer::setRenderState(RenderState * ps)
{
	return;
}

//int perMat = 1 ... ---- Added to .h file 
void DX12Renderer::submit(Mesh * mesh)
{
	if (this->iPerMat) {
		drawList2[mesh->technique].push_back(mesh);
	}
	else
		drawList.push_back(mesh);
}

void DX12Renderer::frame()
{
	int iFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	
	//Set texture descriptor heap
	ID3D12DescriptorHeap* ppDescriptorHeaps[] = { m_pDHTexture };
	m_ppCommandLists[iFrameIndex]->SetDescriptorHeaps(1, ppDescriptorHeaps);

	m_ppCommandLists[iFrameIndex]->RSSetViewports(1, &m_viewport);
	m_ppCommandLists[iFrameIndex]->RSSetScissorRects(1, &m_rectScissor);
	m_ppCommandLists[iFrameIndex]->SetGraphicsRootSignature(m_pRS);
	
	//first pipeline.
	m_ppCommandLists[iFrameIndex]->SetPipelineState(m_pPSOs[1]);
	//texture table
	m_ppCommandLists[iFrameIndex]->SetGraphicsRootDescriptorTable(0, m_pDHTexture->GetGPUDescriptorHandleForHeapStart());

	m_ppCommandLists[iFrameIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	float color[4];
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 1.0f;
	color[3] = 0.0f;
	//color constants
	m_ppCommandLists[iFrameIndex]->SetGraphicsRoot32BitConstants(1, 4, color, 0);

	int testttt = drawList2.size();
	
	for (auto work : drawList2)
	{

		std::vector<Mesh*> m = work.second;
		VertexBufferDX12* test = (VertexBufferDX12*)m[0]->geometryBuffers[0].buffer;
		VertexBufferDX12* test1 = (VertexBufferDX12*)m[0]->geometryBuffers[1].buffer;
		VertexBufferDX12* test2 = (VertexBufferDX12*)m[0]->geometryBuffers[2].buffer;
		test->bind(m_ppCommandLists[iFrameIndex], 2);
		test1->bind(m_ppCommandLists[iFrameIndex], 3);
		test2->bind(m_ppCommandLists[iFrameIndex], 4);
			
		int j = 0;
		for (int i = j; i < m.size(); i += 4)
		{
			m_ppCommandLists[iFrameIndex]->DrawInstanced(m[i]->geometryBuffers[0].numElements, 1, 0, 0);
		}
		j++;
		m_ppCommandLists[iFrameIndex]->SetPipelineState(m_pPSOs[0]);
		color[0] = 0.0f;
		color[1] = 1.0f;
		color[2] = 0.0f;
		color[3] = 1.0f;
		//color constants
		m_ppCommandLists[iFrameIndex]->SetGraphicsRoot32BitConstants(1, 4, color, 0);
		for (int i = j; i < m.size(); i += 4)
		{
			m_ppCommandLists[iFrameIndex]->DrawInstanced(m[i]->geometryBuffers[0].numElements, 1, 0, 0);
		}
		j++;

		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 0.0f;//<<----- This way we know it's a texture
		//color constants
		m_ppCommandLists[iFrameIndex]->SetGraphicsRoot32BitConstants(1, 4, color, 0);
		for (int i = j; i < m.size(); i += 4)
		{
			m_ppCommandLists[iFrameIndex]->DrawInstanced(m[i]->geometryBuffers[0].numElements, 1, 0, 0);
		}

		j++;

		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 1.0f;
		//color constants
		m_ppCommandLists[iFrameIndex]->SetGraphicsRoot32BitConstants(1, 4, color, 0);
		for (int i = j; i < m.size(); i += 4)
		{
			m_ppCommandLists[iFrameIndex]->DrawInstanced(m[i]->geometryBuffers[0].numElements, 1, 0, 0);
		}
	}
	drawList2.clear();

}

