#pragma once
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9d")

#define WIN32_LEAN_AND_MEAN

// d3d9
#include <d3d9.h>

// windows
#include <Windows.h>
#include <windowsx.h>
#include <wrl.h>

// c++
#include <iostream>
#include <memory>


#define TMP_ERRORCATCH(e) { \
std::string str = "@@@ Error: " + std::string(e); \
throw std::runtime_error(str.c_str()); \
} \

#define IFTHROWFAILED(e, f) { \
	if(FAILED(f)) { \
		TMP_ERRORCATCH(e); \
	} \
} \


#define MODE_1 1080, 720, false
#define MODE_2 1920, 1080, true

class D3D9App {
public:
	D3D9App();
	D3D9App& operator=(const D3D9App&) = delete;
	D3D9App(const D3D9App&) = delete;
	~D3D9App();

	void Initialize(unsigned int width, unsigned int height, bool fullscreen);
	WPARAM Render();

	LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);


	static D3D9App* st_D3D9App;

protected:
	virtual void startup() = 0;
	virtual void build() = 0;
	virtual void render() = 0;

private:
	void WindowInitialize(unsigned int width, unsigned int height, bool fullscreen);
	void InitializeD3DPresent();
	void DirectX9Initialize();

private:
	HWND m_Hwnd;
	HINSTANCE m_Instance;
	UINT m_ScreenWidth, m_ScreenHeight;
	UINT m_XPos, m_YPos;
	bool m_IsFullscreen;

protected:
	D3DPRESENT_PARAMETERS m_D3DParams;
	Microsoft::WRL::ComPtr<IDirect3D9> m_Direct3D9;
	Microsoft::WRL::ComPtr<IDirect3DDevice9> m_Device;

};

LRESULT __stdcall WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);