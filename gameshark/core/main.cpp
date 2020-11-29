#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "menu\config\config.h"

DWORD WINAPI OnDLLAttach(LPVOID lpParameter) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("gameshark console");
#endif

	try
	{
		interfaces::initialize();
		render::initialize();
		hooks::initialize();

		C::Setup("default.cfg");

		interfaces::engine->execute_cmd("clear");
		interfaces::console->console_color_printf(Color(0, 255, 0), "fucking shit");
	}
	catch (const std::runtime_error& error) 
	{
		MessageBoxA(nullptr, error.what(), "gameshark fucked up!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), 0);
	}
}

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD dwReason, const LPVOID lpReserved) {
	DisableThreadLibraryCalls(hModule);

	switch (dwReason) {
		case DLL_PROCESS_ATTACH: {
			const auto hThread = CreateThread(nullptr, 0U, OnDLLAttach, hModule, 0UL, nullptr);
			if (!hThread)
				return FALSE;

			g::dll = hModule;

			CloseHandle(hThread);

			return TRUE;
		}
		break;
		default:
			break;
	}

	return FALSE;
}
