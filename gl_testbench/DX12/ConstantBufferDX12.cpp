#include "ConstantBufferDX12.h"


ConstantBufferDX12::ConstantBufferDX12(D3DFactory* pFactory)
{
    _pFactory = pFactory;
    m_pLastMat = nullptr;

    //Better in constructor? Can't get size in constructor
    _pFactory->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(float)*4), //hardcode size so we can place it in constructor
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_pVBUpload));
}

ConstantBufferDX12::~ConstantBufferDX12()
{
}

void ConstantBufferDX12::setData(const void * data, size_t size, Material * m, unsigned int location)
{
    //location does nothing in this function? 
    //What is Matieral* m used for?

    D3D12_RANGE range;
    range.Begin = 0;
    range.End = 0; 

    uint8_t* address;
    m_pVBUpload->Map(0, &range, reinterpret_cast<void**>(&address));
    memcpy(address, data, size);
}

void ConstantBufferDX12::bind(Material * m)
{
}
