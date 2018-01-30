#include "D3DFactory.h"



D3DFactory::D3DFactory()
{
	HRESULT hr;

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_pDXGIFactory));
	assert(hr == S_OK);

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

		pAdapter = pTempAdapter;
		hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			iAdapterFound++;
			size_t iVideoMemory = desc1Adapter.DedicatedVideoMemory;
			
			if (iVideoMemory > iBestVideoMemory)
				iBestVideoMemory = iVideoMemory;
		}

		iAdapterID++;
	}

	assert(iAdapterFound > 0);
	hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_pDevice));
	assert(hr == S_OK);
}

D3DFactory::~D3DFactory()
{
	if (m_pDevice)
		m_pDevice->Release();
	if (m_pDXGIFactory)
		m_pDXGIFactory->Release();
}

ID3D12CommandQueue * D3DFactory::CreateCQ()
{
	ID3D12CommandQueue* pCQ;

	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pCQ));

	return pCQ;
}

ID3D12CommandAllocator * D3DFactory::CraeteCA()
{
	ID3D12CommandAllocator* pCA;

	m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCA));

	return pCA;
}

ID3D12GraphicsCommandList * D3DFactory::CreateCL(ID3D12CommandAllocator * pCA)
{
	ID3D12GraphicsCommandList* pCL;

	m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCA, nullptr, IID_PPV_ARGS(&pCL));

	return pCL;
}

ID3D12Fence * D3DFactory::CreateFence()
{
	ID3D12Fence* pFence;

	m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence));

	return pFence;
}

ID3D12RootSignature * D3DFactory::CreateRS(D3D12_ROOT_SIGNATURE_DESC* pDesc)
{
	ID3DBlob* pBlob;
	ID3D12RootSignature* pRS;

	D3D12SerializeRootSignature(pDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pBlob, nullptr);
	m_pDevice->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pRS));

	return pRS;
}

ID3D12PipelineState * D3DFactory::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC * pDesc)
{
	ID3D12PipelineState* pPSO;

	m_pDevice->CreateGraphicsPipelineState(pDesc, IID_PPV_ARGS(&pPSO));

	return pPSO;
}

IDXGISwapChain3 * D3DFactory::CreateSwapChain(DXGI_SWAP_CHAIN_DESC * pDesc, ID3D12CommandQueue * pCQ)
{
	IDXGISwapChain*		pTemp = nullptr;
	IDXGISwapChain3*	pSwapChain = nullptr;

	m_pDXGIFactory->CreateSwapChain(pCQ, pDesc, &pTemp);

	pSwapChain = static_cast<IDXGISwapChain3*>(pTemp);

	return pSwapChain;
}



