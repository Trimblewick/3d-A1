#pragma once
#include "../Sampler2D.h"


class Sampler2DDX12 : public Sampler2D
{
public:
	Sampler2DDX12();
	~Sampler2DDX12();

	void setMagFilter(FILTER filter);
	void setMinFilter(FILTER filter);
	void setWrap(WRAPPING s, WRAPPING t);

private:
	
};
