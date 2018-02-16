#include "RenderStateDX12.h"


RenderStateDX12::RenderStateDX12()
{
	_wireframe = false;
	globalWireFrame = nullptr;
}

RenderStateDX12::~RenderStateDX12()
{
}

void RenderStateDX12::setWireFrame(bool wireframe)
{
	_wireframe = wireframe;
}

void RenderStateDX12::set()
{
	if (*this->globalWireFrame == _wireframe)
		return;
	else
		*this->globalWireFrame = _wireframe;

	if (_wireframe)
		int placeholder1 = 0;//change to wireframe
	else
		int placeholder2 = 0;//change to solid color
}

void RenderStateDX12::setGlobalWireFrame(bool * global)
{
	this->globalWireFrame = global;
}