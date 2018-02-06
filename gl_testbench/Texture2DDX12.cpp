#include "DX12\Texture2DDX12.h"
#include "stb_image.h"

Texture2DDX12::Texture2DDX12() {}

Texture2DDX12::~Texture2DDX12()
{
	if (textureHandle != 0)
	{
		delete[] & textureHandle;
		std::cout << "Error: Texture deleted." << std::endl;
	}
}

//return 0 if image was loaded and texture created
int Texture2DDX12::loadFromFile(std::string filename)
{
	int w, h, bpp;
	unsigned char* rgb = stbi_load(filename.c_str(), &w, &h, &bpp, STBI_rgb_alpha);


	if (rgb == nullptr) //Failed to load texture file
	{
		std::cout << "Error loading texture file: %s\n" << std::endl, filename.c_str();
		return -1;
	}

	if (textureHandle)
	{
		delete[] & textureHandle;
	}


	return 0;
}

void Texture2DDX12::bind(unsigned int slot)
{
}
