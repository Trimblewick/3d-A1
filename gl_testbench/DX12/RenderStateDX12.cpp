#include "RenderStateDX12.h"
#include "RenderStateDX12.h"
#include "RenderStateDX12.h"
#include "RenderStateDX12.h"

RenderStateDX12::RenderStateDX12() 
{
	_wireFrame = false;
	gloablWireFrame = nullptr;
}

RenderStateDX12::~RenderStateDX12()
{

}

void RenderStateDX12::setWireFrame(bool wireframe)
{
	_wireFrame = wireframe;
}

void RenderStateDX12::set()
{
	if (*this->globalWireFrame == _wireFrame)
		return;
	else
		*this->globalWireFrame = _wireFrame;

	if (_wireFrame)
		int placeholder1 = 0;//change to wireframe
	else
		int placeholder2 = 0;//change to solid color
}

void RenderStateDX12::setGlobalWireFrame(bool * global)
{
	this->globalWireFrame = global;
}
