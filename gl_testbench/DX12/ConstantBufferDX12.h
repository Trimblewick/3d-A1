#pragma once
#include "../ConstantBuffer.h"
#include "D3DFactory.h"
#include <math.h>
#include "MaterialDX12.h"

typedef union {
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
} float4;
class ConstantBufferDX12 : public ConstantBuffer
{
public:
	ConstantBufferDX12();
	~ConstantBufferDX12();

	void setData(const void* data, size_t size, Material* m, unsigned int location);
	void bind(Material* m);
	float4 getTranslation();
	void setDiffuse();

private:
    std::string m_name;
    int m_location;
    void* m_pLastMat;//maybe needed
	float4 m_translations;
	float4 diffuse;

};
