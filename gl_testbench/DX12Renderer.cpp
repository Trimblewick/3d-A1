#include "DX12Renderer.h"


DX12Renderer::DX12Renderer()
{
}

DX12Renderer::~DX12Renderer()
{
}


LRESULT CALLBACK DX12Renderer::EventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//If one case is hit the code will execute everything down until a break;

	switch (message)
	{

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hWnd);
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Material * DX12Renderer::makeMaterial(const std::string & name)
{
	return nullptr;
}

Mesh * DX12Renderer::makeMesh()
{
	return nullptr;
}

VertexBuffer * DX12Renderer::makeVertexBuffer(size_t size, VertexBuffer::DATA_USAGE usage)
{
	return nullptr;
}

Texture2D * DX12Renderer::makeTexture2D()
{
	return nullptr;
}

Sampler2D * DX12Renderer::makeSampler2D()
{
	return nullptr;
}

RenderState * DX12Renderer::makeRenderState()
{
	return nullptr;
}

std::string DX12Renderer::getShaderPath()
{
	return std::string();
}

std::string DX12Renderer::getShaderExtension()
{
	return std::string();
}

ConstantBuffer * DX12Renderer::makeConstantBuffer(std::string NAME, unsigned int location)
{
	return nullptr;
}

Technique * DX12Renderer::makeTechnique(Material *, RenderState *)
{
	return nullptr;
}

int DX12Renderer::initialize(unsigned int width, unsigned int height)
{
	m_pWindow = new Window(width, height, EventHandler);



	return 0;
}

void DX12Renderer::setWinTitle(const char * title)
{
	size_t wn = mbsrtowcs(NULL, &title, 0, NULL);
	wchar_t * buf = new wchar_t[wn + 1]();
	mbsrtowcs(buf, &title, wn + 1, NULL);
	

	m_pWindow->SetTitle(buf);
	delete[] buf;
}

void DX12Renderer::present()
{
}

int DX12Renderer::shutdown()
{
	return 0;
}

void DX12Renderer::setClearColor(float r, float g, float b, float a)
{
	m_pClearColor[0] = r;
	m_pClearColor[1] = g;
	m_pClearColor[2] = b;
	m_pClearColor[3] = a;
}

void DX12Renderer::clearBuffer(unsigned int)
{
}

void DX12Renderer::setRenderState(RenderState * ps)
{
}

void DX12Renderer::submit(Mesh * mesh)
{
}

void DX12Renderer::frame()
{
}
