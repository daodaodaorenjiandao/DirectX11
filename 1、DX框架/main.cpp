#include "SystemClass.h"
#include <windows.h>


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	SystemClass* systemClass = SystemClass::Instance();

	if(systemClass->Initialize() == false)
		return 1;

	systemClass->Run();

	systemClass->Shutdown();

	return 0;
}