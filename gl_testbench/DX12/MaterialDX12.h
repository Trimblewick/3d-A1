#pragma once
#include "../Material.h"
#include "DX12Renderer.h"
//#include "ConstantBufferDX12.h"

class D3DFactory;

class MaterialDX12 : public Material
{
	friend D3DFactory;
public:
	//MaterialDX12(D3DFactory* factory);
	MaterialDX12();
	MaterialDX12(std::string name, DX12Renderer* renderer, D3DFactory* factory);
	~MaterialDX12();


	void setShader(const std::string& shaderFileName, ShaderType type);

	// removes any resource linked to shader type
	void removeShader(ShaderType type);

	void setDiffuse(Color c);

	/*
	* Compile and link all shaders
	* Returns 0  if compilation/linking succeeded.
	* Returns -1 if compilation/linking fails.
	* Error is returned in errString
	* A Vertex and a Fragment shader MUST be defined.
	* If compileMaterial is called again, it should RE-COMPILE the shader
	* In principle, it should only be necessary to re-compile if the defines set
	* has changed.
	*/
	int compileMaterial(std::string& errString);

	ID3D12PipelineState* GetPSO();

	// this constant buffer will be bound every time we bind the material
	void addConstantBuffer(std::string name, unsigned int location);

	// location identifies the constant buffer in a unique way
	void updateConstantBuffer(const void* data, size_t size, unsigned int location);

	//std::map<unsigned int, ConstantBufferDX12> constantBuffers;

	// activate the material for use.
	int enable();

	// disable material
	void disable();

private:
	std::string m_mapShaderEnum[4];
	std::string m_pName;
	//int program;
	int m_pShaderObjects[4] = { 0 };
	std::string m_pShaderNames[4];
	D3DFactory* _pFactory;
	DX12Renderer* _pRenderer;
	ID3D12PipelineState* m_pPSO;
};