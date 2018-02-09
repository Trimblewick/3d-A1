#pragma once
#include "DX12\VertexBufferDX12.h"
#include "DX12\MeshDX12.h"
#include <assert.h>

VertexBufferDX12::VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage, DX12Renderer* Renderer,D3DFactory* mFactory)
{
	totalSize = size;
	m_pFactory = mFactory;
	m_pRenderer = Renderer;

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(size_t) * size);
	
	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	m_pFactory->GetDevice()->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vbDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_pVertexBuffer));

	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	m_pFactory->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vbDesc,
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

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = data;
	vertexData.RowPitch = size;
	vertexData.SlicePitch = vertexData.RowPitch; //?
 
	int index = 0;
	UpdateSubresources(m_pRenderer->GetCMDL(index), m_pVertexBuffer, m_pVBUpload, 0/*offset?*/, 0, size, &vertexData);
	//unbind()?
	//Resource barrier?
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