#pragma once

#include "D3DFactory.h"
#include "../Texture2D.h"
#include "Sampler2DDX12.h"
#include <wincodec.h>

class Texture2DDX12 : public Texture2D
{
private:
	ID3D12Resource*						m_pResourceTexture;
	ID3D12DescriptorHeap*				_pDHTexture;

	/*BYTE*								m_pTextureData;
	int									m_iBytesPerRow;
	D3D12_RESOURCE_DESC					m_textureDesc;
	D3D12_SHADER_RESOURCE_VIEW_DESC		m_descSRV;

	int									m_iTextureSize;

	unsigned int						m_iWidth;
	unsigned int						m_iHeight;*/

	D3DFactory*							_pD3DFactory;
public:
	Texture2DDX12(D3DFactory* pD3DFactory, ID3D12DescriptorHeap* pDHTexture);
	~Texture2DDX12();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);
private:
	//for decoding
	DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID);
	WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
	int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
};
