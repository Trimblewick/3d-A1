#pragma once
#include "../Texture2D.h"
#include "Sampler2DDX12.h"
#include <iostream>


class Texture2DDX12 : public Texture2D
{
public:
	Texture2DDX12();
	~Texture2DDX12();

	int loadFromFile(std::string filename);
	void bind(unsigned int slot);

	unsigned int textureHandle = 0;
};