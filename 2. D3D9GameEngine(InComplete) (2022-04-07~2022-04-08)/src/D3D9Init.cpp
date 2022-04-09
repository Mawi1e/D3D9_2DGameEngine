#include "D3D9Init.h"

void D3D9Init::startup() {

}

void D3D9Init::build() {

}

void D3D9Init::render(float d) {
	m_Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 255), 0.0f, 0);

	m_Device->BeginScene();
	m_Device->EndScene();

	m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}