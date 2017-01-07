#include "SystemClass.h"
#include "InputClass.h"
#include "GraphicsClass.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

SystemClass::SystemClass()
	:m_input(nullptr),
	m_graphics(nullptr),
	m_applicationName(APPLICATION_NAME),
	m_windowClassName(WINDOW_CLASS_NAME),
	m_hWnd(NULL),
	m_hInstance(NULL)
{

}


SystemClass::~SystemClass()
{
	Shutdown();
}

SystemClass* SystemClass::Instance()
{
	static SystemClass systemClass;

	return std::addressof(systemClass);
}

bool SystemClass::Initialize()
{
	int width, height;

	width = 0;
	height = 0;

	//1 init window
	InitializeWindows(width, height);

	//2 create InputClass obj and init it
	m_input = new InputClass;
	if(m_input == nullptr)
		return false;
	else
		m_input->Initialize();

	//3 create GraphicsClass obj and init it
	m_graphics = new GraphicsClass;

	if(m_graphics == nullptr)
		return false;
	if(m_graphics->Initialize(m_hWnd, width, height) == false)
		return false;

	return true;
}

void SystemClass::Shutdown()
{
	if(m_graphics)
	{
		delete m_graphics;
		m_graphics = nullptr;
	}

	if(m_input)
	{
		delete m_input;
		m_input = nullptr;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while(true)
	{
		if(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;

			if(Frame() == false)
				break;
		}
	}
}

bool SystemClass::Frame()
{
	if(m_input->IsKeyDown(VK_ESCAPE))
		return false;

	if(m_graphics->Frame() == false)
		return false;

	return true;
}

LRESULT /*CALLBACK*/ SystemClass::MessageHandler(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_input->KeyRelease((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hWnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get the instance of this application.
	m_hInstance = GetModuleHandle(NULL);

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowClassName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		screenWidth = SCREEN_WIDTH;
		screenHeight = SCREEN_HEIGHT;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, 
							m_windowClassName.c_str(),
							m_applicationName.c_str(),
							WS_OVERLAPPEDWINDOW,
							posX, posY, 
							screenWidth, screenHeight, 
							NULL, 
							NULL, 
							m_hInstance, 
							NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_windowClassName.c_str(), m_hInstance);
	m_hInstance = NULL;


	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return SystemClass::Instance()->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}