#pragma once

#include "../Renderer.h"
#include "D3DFactory.h"
#include "Window.h"

const unsigned int g_iBackBufferCount = 3;
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

class MaterialDX12;

class DX12Renderer : public Renderer
{
private:
	static LRESULT CALLBACK EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	Window*								m_pWindow;
	D3DFactory*							m_pD3DFactory;


	ID3D12CommandQueue*					m_pCommandQueue;
	IDXGISwapChain3*					m_pSwapChain;
	float								m_pClearColor[4];

	ID3D12DescriptorHeap*				m_pDHrenderTargets;
	ID3D12Resource*						m_ppRenderTargets[g_iBackBufferCount];

	unsigned long long					m_pFenceValues[g_iBackBufferCount];
	ID3D12Fence*						m_ppFenceFrame[g_iBackBufferCount];
	HANDLE								m_handleFence;

	ID3D12CommandAllocator*				m_ppCommandAllocators[g_iBackBufferCount];
	ID3D12GraphicsCommandList*			m_ppCommandLists[g_iBackBufferCount];

	ID3D12RootSignature*				m_pRS;

	D3D12_VIEWPORT						m_viewport;
	D3D12_RECT							m_rectScissor;

	//MaterialDX12*  					m_pMaterial;
	std::vector<Mesh*> drawList;


	bool globalWireframeMode = false;

public:
	DX12Renderer();
	~DX12Renderer();

	ID3D12RootSignature* GetRS();
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



