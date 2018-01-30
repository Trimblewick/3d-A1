#pragma once

#include "Renderer.h"
#include "D3DFactory.h"
#include "Window.h"


class DX12Renderer : public Renderer
{
private:
	static LRESULT CALLBACK EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Window* m_pWindow;
	D3DFactory* m_pD3DFactory;

	float m_pClearColor[4];

public:
	DX12Renderer();
	~DX12Renderer();



private:


	// Inherited via Renderer
	Material * makeMaterial(const std::string & name);

	Mesh * makeMesh();

	VertexBuffer * makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage);

	Texture2D * makeTexture2D();

	Sampler2D * makeSampler2D();

	RenderState * makeRenderState();

	std::string getShaderPath();

	std::string getShaderExtension();

	ConstantBuffer * makeConstantBuffer(std::string NAME, unsigned int location);

	Technique * makeTechnique(Material *, RenderState *);

	int initialize(unsigned int width = 800, unsigned int height = 600);

	void setWinTitle(const char * title);

	void present();

	int shutdown();

	void setClearColor(float, float, float, float);

	void clearBuffer(unsigned int);

	void setRenderState(RenderState * ps);

	void submit(Mesh * mesh);

	void frame();

};



