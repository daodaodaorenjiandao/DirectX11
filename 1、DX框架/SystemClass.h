#pragma once
#include <windows.h>
#include <string>

#include "config.h"

class InputClass;
class GraphicsClass;


class SystemClass
{
private:
	SystemClass();
	SystemClass(const SystemClass& other);
	SystemClass(SystemClass&& other);

	SystemClass& operator =(const SystemClass& rh);

public:
	~SystemClass();

	static SystemClass* Instance();

	bool Initialize();

	void Shutdown();

	void Run();

	LRESULT /*CALLBACK*/ MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();

	void InitializeWindows(int&, int&);

	void ShutdownWindows();

private:
	const std::wstring m_applicationName;
	const std::wstring m_windowClassName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	InputClass* m_input;
	GraphicsClass* m_graphics;
};

