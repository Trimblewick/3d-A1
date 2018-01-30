#include "D3DFactory.h"


#include <tchar.h>
#define DxAssert(x, y)	{ if (x != y) { MessageBox(NULL, _T("DxAssert"), _T("ERROR"), MB_OK); assert(x == y); } }

D3DFactory::D3DFactory()
{
	HRESULT hr;
	DxAssert(CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory)), S_OK);

	IDXGIAdapter1* pTempAdapter;
	IDXGIAdapter1* pAdapter;
	int iAdapterID = 0;
	int iAdapterFound = 0;
	size_t iBestVideoMemory = 0;

	while (m_pDXGIFactory->EnumAdapters1(iAdapterID, &pTempAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc1Adapter;
		pTempAdapter->GetDesc1(&desc1Adapter);

		if (desc1Adapter.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			iAdapterID++;
			continue;
		}

		
		hr = D3D12CreateDevice(pTempAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			iAdapterFound++;
			size_t iVideoMemory = desc1Adapter.DedicatedVideoMemory;
			
			if (iVideoMemory > iBestVideoMemory)
			{
				iBestVideoMemory = iVideoMemory;
				pAdapter = pTempAdapter;
			}
		}

		iAdapterID++;
	}

	assert(iAdapterFound > 0);

	DxAssert(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice)), S_OK);
	
}

D3DFactory::~D3DFactory()
{
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pDXGIFactory)
		m_pDXGIFactory->Release();
}

ID3D12Device * D3DFactory::GetDevice()
{
	return m_pDevice;
}

ID3D12CommandQueue * D3DFactory::CreateCQ()
{
	ID3D12CommandQueue* pCQ;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	DxAssert(m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pCQ)), S_OK);

	return pCQ;
}

ID3D12CommandAllocator * D3DFactory::CreateCA()
{
	ID3D12CommandAllocator* pCA;

	DxAssert(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCA)), S_OK);

	return pCA;
}

ID3D12GraphicsCommandList * D3DFactory::CreateCL(ID3D12CommandAllocator * pCA)
{
	ID3D12GraphicsCommandList* pCL;

	DxAssert(m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCA, nullptr, IID_PPV_ARGS(&pCL)), S_OK);

	return pCL;
}

ID3D12Fence * D3DFactory::CreateFence()
{
	ID3D12Fence* pFence;

	DxAssert(m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)), S_OK);

	return pFence;
}

ID3D12RootSignature * D3DFactory::CreateRS(D3D12_ROOT_SIGNATURE_DESC* pDesc)
{
	ID3DBlob* pBlob;
	ID3D12RootSignature* pRS;

	DxAssert(D3D12SerializeRootSignature(pDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pBlob, nullptr), S_OK);
	DxAssert(m_pDevice->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pRS)), S_OK);

	return pRS;
}

ID3D12PipelineState * D3DFactory::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC * pDesc)
{
	ID3D12PipelineState* pPSO;

	DxAssert(m_pDevice->CreateGraphicsPipelineState(pDesc, IID_PPV_ARGS(&pPSO)), S_OK);

	return pPSO;
}

IDXGISwapChain3 * D3DFactory::CreateSwapChain(DXGI_SWAP_CHAIN_DESC desc, ID3D12CommandQueue * pCQ)
{
	IDXGISwapChain*		pTemp = nullptr;
	IDXGISwapChain3*	pSwapChain = nullptr;

	HRESULT hr;
	hr = m_pDXGIFactory->CreateSwapChain(pCQ, &desc, &pTemp);

	pSwapChain = static_cast<IDXGISwapChain3*>(pTemp);

	return pSwapChain;
}

ID3D12DescriptorHeap * D3DFactory::CreateDH(int numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, bool bShaderVisible)
{
	ID3D12DescriptorHeap* pDH;
	
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = numDescriptors;
	desc.Type = type;

	if (bShaderVisible)
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	DxAssert(m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&pDH)), S_OK);

	return pDH;
}



