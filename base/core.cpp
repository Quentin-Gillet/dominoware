// used: sleep_for
#include <thread>

// used: winapi, directx, fmt includes
#include "common.h"
// used: global variables
#include "global.h"
// used: eventlistener setup/destroy
#include "utilities.h"
// used: result logging
#include "utilities/logging.h"
// used: exports setup
#include "utilities/math.h"
// used: inputsystem setup/destroy
#include "utilities/inputsystem.h"
// used: render setup/destroy
#include "utilities/draw.h"
// used: netvar manager setup
#include "core/netvar.h"
// used: hooks setup/destroy
#include "core/hooks.h"
// used: menu
#include "core/menu/menu.h"
// used: skins dump
#include "features/skinchanger.h"

DWORD WINAPI OnDllAttach(LPVOID lpParameter)
{
	try
	{
		/*
		 * @note: serverbrowser.dll is last loaded module (u can seen it when debug)
		 * here is check for all modules loaded
		 */
		while (MEM::GetModuleBaseHandle(SERVERBROWSER_DLL) == nullptr)
			std::this_thread::sleep_for(200ms);

#ifdef DEBUG_CONSOLE
		// console logging
		if (!L::Attach(XorStr("dominoware's developer-mode")))
			throw std::runtime_error(XorStr("failed to attach console"));

		L::Print(XorStr("console opened"));
#else
		// file logging
		// @note: use std::ios::app instead std::ios::trunc to not clear every time
		L::ofsFile.open(C::GetWorkingPath().append(XorStr("dominoware.log")), std::ios::out | std::ios::trunc);
#endif

		// capture interfaces from game/steam (not always) modules
		if (!I::Setup())
			throw std::runtime_error(XorStr("failed to capture interfaces"));

		L::Print(XorStr("interfaces captured"));

		// version check to know when u need to fix something
#ifdef DEBUG_CONSOLE
		if (strcmp(I::Engine->GetProductVersionString(), XorStr("1.38.3.2")) != 0)
		{
			L::PushConsoleColor(FOREGROUND_YELLOW);
			L::Print(XorStr("[warning] version doesnt match! current cs:go version: {}"), I::Engine->GetProductVersionString());
			L::PopConsoleColor();
		}
#endif

		/*
		 * fill networkable variables map
		 * dump received netvars to the file
		 */
		if (!CNetvarManager::Get().Setup(XorStr("netvars.dw")))
			throw std::runtime_error(XorStr("failed to initialize netvars"));

		L::Print(XorStr("found [{:d}] props in [{:d}] tables"), CNetvarManager::Get().iStoredProps, CNetvarManager::Get().iStoredTables);

		// export completed mathematics functions from game/steam (not always) modules
		if (!M::Setup())
			throw std::runtime_error(XorStr("failed to get math exports"));

		L::Print(XorStr("math exports loaded"));

		// init inputs
		IPT::Setup();

		// init draw method
		D::Setup();

		W::Initialize();
		L::Print(XorStr("[MENU] Menu initialized."));
#if 0
		// start tracking entities
		U::EntityListener.Setup();
		L::Print(XorStr("entity listener initialized"));
#endif

		// start tracking specified events from vector
		// @note: all events list: https://wiki.alliedmods.net/Counter-Strike:_Global_Offensive_Events
		U::EventListener.Setup({ XorStr("player_hurt"), XorStr("round_prestart"), XorStr("round_freeze_end") });
		L::Print(XorStr("events registered"));

		// add our functionality in client functions
		if (!H::Setup())
			throw std::runtime_error(XorStr("failed initialize hooks"));

		L::Print(XorStr("hooks setup complete"));

		// add our functionality in networkable functions
		if (!P::Setup())
			throw std::runtime_error(XorStr("failed initialize proxies"));

		L::Print(XorStr("proxies applied"));

		CSkinChanger::Get().Init();

		// show message about successful load in logs and in game console
		L::PushConsoleColor(FOREGROUND_MAGENTA);
		L::Print(XorStr("dominoware successfully loaded"));
		L::PopConsoleColor();
		I::GameConsole->Clear();
		I::ConVar->ConsoleColorPrintf(Color(255, 50, 255, 255), XorStr("dominoware successfully loaded.\nbuild date: %s / %s\n"), __DATE__, __TIME__);
	}
	catch (const std::exception& ex)
	{
		// print error message
		L::PushConsoleColor(FOREGROUND_INTENSE_RED);
		L::Print(XorStr("[error] {}"), ex.what());
		L::PopConsoleColor();

#ifdef _DEBUG
		// show error message window (or replace to your exception handler)
		_RPT0(_CRT_ERROR, ex.what());
#else
		// unload
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParameter), EXIT_FAILURE);
#endif
	}

	return 1UL;
}

DWORD WINAPI OnDllDetach(LPVOID lpParameter)
{
	// unload cheat if pressed specified key
	while (!IPT::IsKeyReleased(S::KEY_END) && !IPT::IsKeyHeld(S::KEY_END))
		std::this_thread::sleep_for(500ms);

#if 0
	// destroy entity listener
	U::EntityListener.Destroy();
#endif

	// destroy events listener
	U::EventListener.Destroy();

	// reset crosshair state
	I::ConVar->FindVar(XorStr("crosshair"))->SetValue(true);

	// restore proxies
	P::Restore();

	// restore hooks
	H::Restore();

#ifdef DEBUG_CONSOLE
	// detach console
	L::Detach();
#else
	// close logging output file
	if (L::ofsFile.is_open())
		L::ofsFile.close();
#endif

	// free our library memory from process and exit from our thread
	FreeLibraryAndExitThread((HMODULE)lpParameter, EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH reasons to call
		DisableThreadLibraryCalls(hModule);

		// basic process check
		if (MEM::GetModuleBaseHandle(XorStr("csgo.exe")) == nullptr)
		{
			MessageBox(nullptr, XorStr("this cannot be injected in another process\nopen <csgo.exe> to inject"), XorStr("dominoware"), MB_OK);
			return FALSE;
		}

		// save our module
		G::hDll = hModule;

		// create main thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllAttach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		// create detach thread
		if (auto hThread = CreateThread(nullptr, 0U, OnDllDetach, hModule, 0UL, nullptr); hThread != nullptr)
			CloseHandle(hThread);

		return TRUE;
	}

	return FALSE;
}
