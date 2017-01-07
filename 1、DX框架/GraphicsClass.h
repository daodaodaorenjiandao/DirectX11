#pragma once
#include <windows.h>
#include "config.h"

class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(HWND hWnd, int screenWidth, int screenHeight);

	void ShutDown();

	bool Frame();

private:
	bool Render();
};

