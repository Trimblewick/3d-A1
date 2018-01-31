#include "Window.h"

Window::Window(unsigned int iWidth, unsigned int iHeight, WNDPROC EventHandler)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;


	m_windowInstance = GetModuleHandle(0);

	m_windowInfo = { 0 };
	m_windowInfo.cbSize = sizeof(WNDCLASSEX);
	m_windowInfo.style = CS_HREDRAW | CS_VREDRAW;
	m_windowInfo.lpfnWndProc = EventHandler;
	m_windowInfo.hInstance = m_windowInstance;
	m_windowInfo.lpszClassName = L"Window";
	m_windowInfo.hIcon = LoadIcon(m_windowInstance, MAKEINTRESOURCE(101));

	if (RegisterClassEx(&m_windowInfo))
	{
		int stoppper = 0;
	}

	m_windowRect = { 0, 0, (long)iWidth, (long)iHeight };
	AdjustWindowRect(&m_windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_windowHandle = CreateWindow(
		L"Window",
		L"Window",	
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_windowRect.right - m_windowRect.left,
		m_windowRect.bottom - m_windowRect.top,
		nullptr,
		nullptr,
		m_windowInstance,
		nullptr);

	ShowWindow(m_windowHandle, 10);

}

Window::~Window()
{
	//DestroyWindow(m_windowHandle);
}

int Window::GetWidth()
{
	return m_iWidth;
}

int Window::GetHeight()
{
	return m_iHeight;
}

HWND Window::GetWindowHandle()
{
	return m_windowHandle;
}

HINSTANCE Window::GetWindowInstance()
{
	return m_windowInstance;
}

void Window::SetTitle(LPCWSTR title)
{
	SetWindowText(m_windowHandle, title);
}
