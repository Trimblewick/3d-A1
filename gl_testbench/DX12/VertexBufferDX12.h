#pragma once
#include "../VertexBuffer.h"
#include "d3dx12.h"
#include "DX12Renderer.h"
#include "D3DFactory.h"

class VertexBufferDX12 : public VertexBuffer
{
public:

	VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage, D3DFactory* Factory);
	~VertexBufferDX12();

	void setData(const void* data, size_t size, size_t offset);
	void bind(ID3D12GraphicsCommandList* pCL, int iParameterIndex);
	void unbind();
	size_t getSize();

	bool bLameAssHack;
	static int usageMapping[3];

private:
	size_t totalSize;
	int _handle;

	ID3D12DescriptorHeap*		m_pDHverts;

	ID3D12Resource*				m_pVBUpload;
	D3DFactory*					_pFactory;
};

