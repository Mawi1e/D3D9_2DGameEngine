#include "D3D9GameEngine.h"

void D3D9GameEngine::startup() {

}

void D3D9GameEngine::build() {

}

void D3D9GameEngine::render(float d) {
	m_Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 0.0f, 0);

	m_Device->BeginScene();
	m_Device->EndScene();

	m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}