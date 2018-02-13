#pragma once
#include "VertexBufferDX12.h"
#include "MeshDX12.h"
#include <assert.h>

VertexBufferDX12::VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage, D3DFactory* pFactory)
{
	totalSize = size;
	_pFactory = pFactory;

	_pFactory->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_pVBUpload));

}

VertexBufferDX12::~VertexBufferDX12()
{

}

void VertexBufferDX12::setData(const void * data, size_t size, size_t offset)
{
	assert(size + offset <= totalSize);
	D3D12_RANGE range;
	range.Begin = offset;
	range.End = (size + offset);
 
	uint8_t* adress;
	m_pVBUpload->Map(0, &range, reinterpret_cast<void**>(&adress));
	memcpy(adress, data, size);
}

void VertexBufferDX12::bind(size_t offset, size_t size, unsigned int location)
{
	assert(size + offset <= totalSize);
}

void VertexBufferDX12::unbind()
{
	//unbind
}

size_t VertexBufferDX12::getSize()
{
	return totalSize;
}