#pragma once
#include "../VertexBuffer.h"
#include "d3dx12.h"

class VertexBufferDX12 : public VertexBuffer
{
public:

	VertexBufferDX12(size_t size, VertexBuffer::DATA_USAGE usage);
	~VertexBufferDX12();

	void setData(const void* data, size_t size, size_t offset);
	void bind(size_t offset, size_t size, unsigned int location);
	void unbind();
	size_t getSize();

	static int usageMapping[3];

private:
	size_t totalSize;
	int _handle;
};

