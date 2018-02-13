#pragma once
#include "VertexBufferDX12.h"
#include "MeshDX12.h"
#include <assert.h>

VertexBufferDX12::VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage, D3DFactory* pFactory)
{
	m_pDHverts = nullptr;
	m_pVBUpload = nullptr;
	totalSize = size;
	_pFactory = pFactory;

	_pFactory->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pVBUpload));

	m_pDHverts = _pFactory->CreateDH(1, D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);

	bLameAssHack = true;
}

VertexBufferDX12::~VertexBufferDX12()
{
	SAFE_RELEASE(m_pDHverts);
	SAFE_RELEASE(m_pVBUpload);
}

void VertexBufferDX12::setData(const void * data, size_t size, size_t offset)
{
	if (bLameAssHack)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC descSRV = {};
		descSRV.Buffer.NumElements = totalSize / size;
		descSRV.Buffer.StructureByteStride = size;
		descSRV.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		descSRV.Format = DXGI_FORMAT_UNKNOWN;
		descSRV.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

		_pFactory->GetDevice()->CreateShaderResourceView(m_pVBUpload, &descSRV, m_pDHverts->GetCPUDescriptorHandleForHeapStart());

		bLameAssHack = false;
	}
	assert(size + offset <= totalSize);
	D3D12_RANGE range;
	range.Begin = offset / size;
	range.End = (size + offset) / size;
 
	uint8_t* adress; //address*
	m_pVBUpload->Map(0, &range, reinterpret_cast<void**>(&adress));
	memcpy(adress, data, size);
	m_pVBUpload->Unmap(0, &range);
}

void VertexBufferDX12::bind(ID3D12GraphicsCommandList* pCL, int iParameterIndex)
{
	pCL->SetGraphicsRootShaderResourceView(iParameterIndex, m_pVBUpload->GetGPUVirtualAddress());
}

void VertexBufferDX12::unbind()
{
	//unbind
}

size_t VertexBufferDX12::getSize()
{
	return totalSize;
}