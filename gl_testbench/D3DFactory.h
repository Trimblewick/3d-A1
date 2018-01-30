#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <assert.h>



class D3DFactory
{
public:
	D3DFactory();
	~D3DFactory();

	ID3D12CommandQueue*						CreateCQ();
	ID3D12CommandAllocator*					CraeteCA();
	ID3D12GraphicsCommandList*				CreateCL(ID3D12CommandAllocator* pCA);
	ID3D12Fence*							CreateFence();
	ID3D12RootSignature*					CreateRS(D3D12_ROOT_SIGNATURE_DESC* pDesc);
	ID3D12PipelineState*					CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc);
	IDXGISwapChain3*						CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc, ID3D12CommandQueue* pCQ);
	

private:
	IDXGIFactory4*							m_pDXGIFactory;
	ID3D12Device*							m_pDevice;
};
