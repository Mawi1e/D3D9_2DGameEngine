#include "D3D9Init.h"

int __stdcall WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	std::unique_ptr<D3D9Init> d3d9App(new D3D9Init);
	WPARAM wpFlag;

	try {
		d3d9App->Initialize(MODE_1);
		wpFlag = d3d9App->Render();
	}
	catch (const std::exception& e) {
		MessageBoxA(0, e.what(), "## WARNING !! ##", MB_ICONWARNING);
		return -1;
	}

	return wpFlag;
}