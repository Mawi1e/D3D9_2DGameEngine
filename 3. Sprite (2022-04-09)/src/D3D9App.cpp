#include "D3D9App.h"

static LPCWSTR st_WndClassName = L"Mawile's App";
D3D9App* D3D9App::st_D3D9App = nullptr;

LRESULT __stdcall WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	return D3D9App::st_D3D9App->MessageHandler(hwnd, msg, wp, lp);
}

D3D9App::D3D9App() {
	st_D3D9App = this;
	m_GameTimer = std::make_unique<GameTimer>();
}

D3D9App::~D3D9App() {
	ShowCursor(TRUE);
}

void D3D9App::Initialize(unsigned int width, unsigned int height, bool fullscreen) {
	startup();

	WindowInitialize(width, height, fullscreen);
	DirectX9Initialize();

	build();
}
void D3D9App::WindowInitialize(unsigned int width, unsigned int height, bool fullscreen) {
	DEVMODE devMode = {};
	WNDCLASSEXW wndClass = {};

	m_ScreenWidth = width;
	m_ScreenHeight = height;
	m_IsFullscreen = fullscreen;

	m_Instance = GetModuleHandle(NULL);

	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	wndClass.hIconSm = wndClass.hIcon;
	wndClass.hInstance = m_Instance;
	wndClass.lpszClassName = st_WndClassName;
	wndClass.lpszMenuName = 0;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WindowProcedure;

	RegisterClassExW(&wndClass);

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	DWORD flag = fullscreen ? (WS_VISIBLE | WS_POPUP) : (WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	if (fullscreen) {
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = m_ScreenWidth = screenWidth;
		devMode.dmPelsHeight = m_ScreenHeight = screenHeight;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);

		m_XPos = m_YPos = 0;
	}
	else {
		m_XPos = (screenWidth - m_ScreenWidth) / 2;
		m_YPos = (screenHeight - m_ScreenHeight) / 2;
	}

	m_Hwnd = CreateWindowExW(0,
		st_WndClassName,
		st_WndClassName,
		flag,
		m_XPos, m_YPos, m_ScreenWidth, m_ScreenHeight, nullptr, nullptr, m_Instance, nullptr);
	if (m_Hwnd == nullptr) {
		TMP_ERRORCATCH("CreateWindowExW");
	}

	ShowWindow(m_Hwnd, SW_SHOW);
	ShowCursor(!fullscreen);
	UpdateWindow(m_Hwnd);
}

long D3D9App::InitializeD3DPresent() {
	long result = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	ZeroMemory(&m_D3DParams, sizeof(m_D3DParams));
	m_D3DParams.BackBufferCount = 2;
	m_D3DParams.BackBufferFormat = m_IsFullscreen ? D3DFMT_X8R8G8B8 : D3DFMT_UNKNOWN;
	m_D3DParams.BackBufferHeight = m_ScreenHeight;
	m_D3DParams.BackBufferWidth = m_ScreenWidth;
	m_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_D3DParams.Windowed = (!m_IsFullscreen);
	m_D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_D3DParams.hDeviceWindow = m_Hwnd;


	if (m_IsFullscreen) {
		if (IsAdapterCompatible()) {
			m_D3DParams.FullScreen_RefreshRateInHz = m_RefreshRate;
		}
		else {
			TMP_ERRORCATCH("Does not found Compatible Adapters...");
		}
	}

	D3DCAPS9 d3dCaps;
	IFTHROWFAILED("IDirect9::GetDeviceCaps", m_Direct3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps));

	if ((d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		d3dCaps.VertexShaderVersion < D3DVS_VERSION(1, 1)) {
		result = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else {
		result = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	return result;
}

void D3D9App::DirectX9Initialize() {
	m_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_Direct3D9 == nullptr) {
		TMP_ERRORCATCH("Direct3DCreate9");
	}

	long createProcessing = InitializeD3DPresent();

	IFTHROWFAILED("IDirect3D9::CreateDevice", m_Direct3D9->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_Hwnd,
		createProcessing,
		&m_D3DParams,
		m_Device.GetAddressOf()));
}

void D3D9App::ShowFps() {
	static float elapsedTime = 0.0f;
	static int fps = 0;

	++fps;
	if (m_GameTimer->getTotalTime() - elapsedTime >= 1.0f) {
		std::wstring str = L"FPS > " + std::to_wstring(fps) + L"m/s";
		SetWindowTextW(m_Hwnd, str.c_str());

		fps = 0;
		elapsedTime += 1.0f;
	}
}

bool D3D9App::IsAdapterCompatible() {
	D3DDISPLAYMODE displayMode;
	UINT modeCount = m_Direct3D9->GetAdapterModeCount(D3DADAPTER_DEFAULT, m_D3DParams.BackBufferFormat);

	for (UINT i = 0; i < modeCount; ++i) {
		m_Direct3D9->EnumAdapterModes(D3DADAPTER_DEFAULT, m_D3DParams.BackBufferFormat, i, &displayMode);

		if (displayMode.Width == m_D3DParams.BackBufferWidth &&
			displayMode.Height == m_D3DParams.BackBufferHeight &&
			displayMode.RefreshRate >= m_D3DParams.FullScreen_RefreshRateInHz) {
			m_RefreshRate = m_D3DParams.FullScreen_RefreshRateInHz;
			return true;
		}
	}

	return false;
}

HRESULT D3D9App::GetDeviceState() {
	return m_Device->TestCooperativeLevel();
}

void D3D9App::Reset() {
	DirectX9Initialize();
	m_Sprite->OnLostDevice();
	IFTHROWFAILED("IDirect3DDevice::Reset", m_Device->Reset(&m_D3DParams));
	m_Sprite->OnResetDevice();
}

void D3D9App::HandleLostGraphicsDevice() {
	HRESULT result = GetDeviceState();

	if (FAILED(result)) {
		if (result == D3DERR_DEVICELOST) {
			Sleep(100);
			return;
		}
		else if (result == D3DERR_DEVICENOTRESET) {
			m_Device.Reset();
			m_Direct3D9.Reset();
			Reset();

			return;
		}
		else return;
	}
}

WPARAM D3D9App::Render() {
	MSG msg = {};

	m_GameTimer->Reset();

	for (;;) {
		if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
			DispatchMessageW(&msg);
			TranslateMessage(&msg);
		}
		else {
			m_GameTimer->Tick();

			ShowFps();
			render(m_GameTimer->getDeltaTime());
			HandleLostGraphicsDevice();
		}

		if (msg.message == WM_QUIT) {
			break;
		}
	}

	return msg.wParam;
}

LRESULT D3D9App::MessageHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	int new_x, new_y;

	switch (msg) {
	case WM_ACTIVATE:
		if (LOWORD(wp) == WA_INACTIVE) {
			m_GameTimer->Start();
		}
		else {
			m_GameTimer->Start();
		}
		return 0;
	case WM_SIZE:
		new_x = LOWORD(lp);
		new_y = HIWORD(lp);

		return 0;
	case WM_ENTERSIZEMOVE:
		m_GameTimer->Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		m_GameTimer->Start();
		return 0;

	case WM_KEYDOWN:
		if (wp == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;
	case WM_DESTROY: PostQuitMessage(0); return 0;
	case WM_CLOSE: PostQuitMessage(0); return 0;
	}

	return DefWindowProcW(hwnd, msg, wp, lp);
}