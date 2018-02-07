#pragma once
#include "../RenderState.h"

class RenderStateDX12 : RenderState
{
public:
	RenderStateDX12();
	~RenderStateDX12();
	void setWireFrame(bool);
	void set();

	void setGlobalWireFrame(bool* global);
private:
	bool _wireFrame;
	bool* globalWireFrame;
};
