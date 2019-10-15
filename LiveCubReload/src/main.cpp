#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include "efsw/efsw.hpp"
#include "CWTCore.h"
#include "CWTCore/internal/Context.h" // TODO: Expose d3d11 globals in CWTCore so that this doesn't need to touch internal.
#include "SpriteIDMap.h"

using namespace CWT;

// Exports for ChrisMiuchiz's mod loader.
extern "C" {
	__declspec(dllexport) void ModPreInitialize() {};

	__declspec(dllexport) int ModMajorVersion() {
		return 4;
	};

	__declspec(dllexport) int ModMinorVersion() {
		return 1;
	};
};

// Open new console for c(in/out/err)
void OpenConsole()
{
	AllocConsole();
	FILE* cinStream;
	FILE* coutStream;
	FILE* cerrStream;
	freopen_s(&cinStream, "CONIN$", "r", stdin);
	freopen_s(&coutStream, "CONOUT$", "w", stdout);
	freopen_s(&cerrStream, "CONOUT$", "w", stderr);
	std::cout.clear();
	std::cin.clear();
	std::cerr.clear();

	// https://stackoverflow.com/a/57241985:
	// std::wcout, std::wclog, std::wcerr, std::wcin
	HANDLE hConOut = CreateFileW(L"CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFileW(L"CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}

class UpdateListener : public efsw::FileWatchListener
{
public:
	UpdateListener() {}

	void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
	{
		if (action == efsw::Actions::Add || action == efsw::Actions::Modified) {
			auto game = CWT::GetGamePtr();

			std::cout << "File: " << filename << " has been added or modified. Reloading..." << std::endl;
	
			// Haven't mapped out which lock goes to what yet, so lock them all.
			EnterCriticalSection(&game->world->world_lock_1);
			EnterCriticalSection(&game->world->world_lock_2);
			EnterCriticalSection(&game->world->world_lock_3);

			try {
				auto new_sprite = cube::Sprite::New(game->world->sprite_manager->gfx_D3D11Graphics);
				auto fp = std::string("sprites/" + filename);
				new_sprite->LoadFromFile(&fp);

				auto sprite_id = cub_to_id[filename];

				delete game->world->sprite_manager->sprites[sprite_id];
				game->world->sprite_manager->sprites[sprite_id] = new_sprite;
			}
			catch (std::exception e) {
				std::cout << e.what() << std::endl;
			}

			LeaveCriticalSection(&game->world->world_lock_3);
			LeaveCriticalSection(&game->world->world_lock_2);
			LeaveCriticalSection(&game->world->world_lock_1);
		}
	}
};

DWORD WINAPI MyFunc(LPVOID lpvParam) 
{
	OpenConsole();
	CWT::InitCWT();

	// Wait until cube::Game is initialized.
	cube::Game* game = nullptr;
	while (game == nullptr) {
		game = CWT::GetGamePtr();
		Sleep(250);
	}

	std::cout << "Got game" << std::endl;

	// Make the filewatcher.
	efsw::FileWatcher* fileWatcher = new efsw::FileWatcher();
	UpdateListener* listener = new UpdateListener();
	efsw::WatchID id = fileWatcher->addWatch("sprites", listener);
	fileWatcher->watch();

	// Busy wait this thread.
	while (true) {
		Sleep(1000);
	}

	return 0;
};

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, MyFunc, 0, 0, NULL);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}