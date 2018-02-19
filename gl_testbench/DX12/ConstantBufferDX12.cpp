#include "ConstantBufferDX12.h"


ConstantBufferDX12::ConstantBufferDX12()
{
}

ConstantBufferDX12::~ConstantBufferDX12()
{
}

void ConstantBufferDX12::setData(const void * data, size_t size, Material * m, unsigned int location)
{
	m_translations = *(float4*)data;
}

void ConstantBufferDX12::bind(Material * m)
{

}

float4 ConstantBufferDX12::getTranslation()
{
	return m_translations;
}

void ConstantBufferDX12::setDiffuse()
{
	return;
}
