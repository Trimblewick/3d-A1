#pragma once

#include <Windows.h>

class Window
{
private:
	unsigned int m_iWidth;
	unsigned int m_iHeight;
	float m_fAspectRatio;
	WNDCLASSEX m_windowInfo;
	HWND m_windowHandle;
	HINSTANCE m_windowInstance;
	RECT m_windowRect;

public:
	Window(unsigned int iWidth, unsigned int iHeight, WNDPROC EventHandler);
	~Window();

	int GetWidth();
	int GetHeight();
	HWND GetWindowHandle();
	HINSTANCE GetWindowInstance();

	void SetTitle(LPCWSTR title);


};







