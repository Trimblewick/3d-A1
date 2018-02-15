#include "MaterialDX12.h"
#include <vector>
#include <assert.h>
#include <fstream>
#include "ConstantBufferDX12.h"

MaterialDX12::MaterialDX12()
{
	m_pPSO = nullptr;
}

MaterialDX12::MaterialDX12(std::string _name, DX12Renderer* renderer, D3DFactory* factory)
{
	isValid = false;
	m_pName = _name;
	_pRenderer = renderer;
	_pFactory = factory;
	m_mapShaderEnum[(int)ShaderType::VS] = "vs";
	m_mapShaderEnum[(int)ShaderType::PS] = "ps";
	m_mapShaderEnum[(int)ShaderType::GS] = "gs";
	m_mapShaderEnum[(int)ShaderType::CS] = "cs";
}

MaterialDX12::~MaterialDX12() {
	SAFE_RELEASE(m_pPSO);
}

void MaterialDX12::setShader(const std::string & shaderFileName, ShaderType type)
{
	return;
	if (shaderFileNames.find(type) != shaderFileNames.end())
	{
		removeShader(type);
	}
	shaderFileNames[type] = shaderFileName;
}

void MaterialDX12::removeShader(ShaderType type)
{
	return;
	int shader = m_pShaderObjects[(int)type];
	//checks whether the shader exists
	if (shaderFileNames.find(type) == shaderFileNames.end())
	{
		assert(shader = 0);
		return;
	}
	else if (shader != 0)
	{
		//remove shader

	}
}

void MaterialDX12::setDiffuse(Color c)
{
}

int MaterialDX12::compileMaterial(std::string & errString)
{
	return 0;
	DXGI_SAMPLE_DESC descSample = {};
	descSample.Count = 1;

	//remove all shaders
	removeShader(ShaderType::VS);
	removeShader(ShaderType::PS);

	// compile shaders
	std::string err;
	std::wstring temp = std::wstring(shaderFileNames[ShaderType::VS].begin(), shaderFileNames[ShaderType::VS].end());
	ID3DBlob* pVSblob = _pFactory->CompileShader((LPCWSTR)temp.c_str(), "main", (LPCSTR)(m_mapShaderEnum[0] + "_5_1").c_str());
	temp = std::wstring(shaderFileNames[ShaderType::PS].begin(), shaderFileNames[ShaderType::PS].end());
	ID3DBlob* pPSblob = _pFactory->CompileShader((LPCWSTR)temp.c_str(), "main", (LPCSTR)(m_mapShaderEnum[1] + "_5_1").c_str());

	m_pShaderObjects[0] = 1;
	m_pShaderObjects[1] = 1;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO = {};
	descPSO.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	descPSO.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	descPSO.NumRenderTargets = 1;
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	descPSO.VS = { pVSblob->GetBufferPointer(), pVSblob->GetBufferSize() };
	descPSO.PS = { pPSblob->GetBufferPointer(), pPSblob->GetBufferSize() };
	descPSO.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	descPSO.SampleDesc = descSample;
	descPSO.SampleMask = 0xffffffff;

	m_pPSO = _pFactory->CreatePSO(&descPSO);

	SAFE_RELEASE(pVSblob);
	SAFE_RELEASE(pPSblob);

	return 0;
}

ID3D12PipelineState * MaterialDX12::GetPSO()
{
	return m_pPSO;
}

void MaterialDX12::addConstantBuffer(std::string name, unsigned int location)
{
	return;
}

void MaterialDX12::updateConstantBuffer(const void * data, size_t size, unsigned int location)
{
	color = *(Color*)data;
	return;
}

int MaterialDX12::enable()
{
	return 0;
}

void MaterialDX12::disable()
{
	return;
}

