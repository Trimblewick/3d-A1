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

