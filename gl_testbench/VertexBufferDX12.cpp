#include "DX12\VertexBufferDX12.h"
#include "DX12\MeshDX12.h"
#include <assert.h>

VertexBufferDX12::VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage)
{
	totalSize = size;
	

	CD3DX12_RESOURCE_DESC vbDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(size_t) * size);

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

	//updateSubresources?

	//bind buffer med data
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
