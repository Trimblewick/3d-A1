#pragma once
#include "../ConstantBuffer.h"
#include "D3DFactory.h"
#include <math.h>
#include "MaterialDX12.h"

class ConstantBufferDX12 : public ConstantBuffer
{
public:
	ConstantBufferDX12(D3DFactory* pFactory);
	~ConstantBufferDX12();

	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material* m);

private:
    D3DFactory* _pFactory;
    ID3D12Resource* m_pVBUpload;
    std::string m_name;
    int m_location;
    void* m_pLastMat;//maybe needed

};