#include "DX12\MaterialDX12.h"
#include "DX12\DX12Renderer.h"
#include <vector>
#include <assert.h>
#include <fstream>

// recursive function to split a string by a delimiter
// easier to read than all that crap using STL...
void split(const char* text, std::vector<std::string>* const temp, const char delim = ' ')
//void split(std::string text, std::vector<std::string>* const temp, const char delim = ' ')
{
	unsigned int delimPos = strcspn(text, (const char*)&delim);
	if (delimPos == strlen(text))
	{
		temp->push_back(std::string(text));
	}
	else {
		temp->push_back(std::string(text, delimPos));
		split(text + delimPos, temp, delim);
	}
	/*
	int pos = text.find(delim, 0);
	if (pos == -1)
	temp->push_back(text);
	else {
	temp->push_back(text.substr(0, pos));
	split(text.substr(pos + 1, text.length() - pos - 1), temp, delim);
	}
	*/
} //shameless copy

MaterialDX12::MaterialDX12(D3DFactory* factory) { m_pFactory = factory; }

MaterialDX12::~MaterialDX12() {}

void MaterialDX12::setShader(const std::string & shaderFileName, ShaderType type)
{
	if (shaderFileNames.find(type) != shaderFileNames.end())
	{
		removeShader(type);
	}
	shaderFileNames[type] = shaderFileName;
}

void MaterialDX12::removeShader(ShaderType type)
{
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
	//remove all shaders
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < m_pShaderObjects[i]; ++j)
		{
			removeShader(ShaderType(i));
		}
	}

	// compile shaders
	std::string err;
	for (int i = 0; i < 4; ++i)
	{
		m_pFactory->CompileShader((LPCWSTR)m_pShaderNames[i].c_str(), "", "");
	}

	return 0;
}

