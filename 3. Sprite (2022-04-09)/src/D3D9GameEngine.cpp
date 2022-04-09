#include "D3D9GameEngine.h"

void D3D9GameEngine::LoadTexture(const char* filepath,
	COLOR_ARGB transcolor, UINT& width, UINT& height, LPDIRECT3DTEXTURE9& texture) {
	D3DXIMAGE_INFO imageInfo;
	D3DXGetImageInfoFromFileA(filepath, &imageInfo);
	D3DXCreateTextureFromFileExA(m_Device.Get(), filepath, imageInfo.Width, imageInfo.Height, 1, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, transcolor, &imageInfo, nullptr, &texture);

	width = imageInfo.Width;
	height = imageInfo.Height;
}

void D3D9GameEngine::drawSprite(SpriteData& spriteData, COLOR_ARGB color) {
	if (spriteData.texture == nullptr) {
		TMP_ERRORCATCH("not found texture file on this path");
	}

	D3DXVECTOR2 spriteCenter(spriteData.width / 2.0f * spriteData.scale, spriteData.height / 2.0f * spriteData.scale);
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);
	D3DXVECTOR2 translate(spriteData.x, spriteData.y);

	if (spriteData.flipHorizontal) {
		scaling.x *= -1;
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);
		translate.x += (float)(spriteData.width * spriteData.scale);
	}

	if (spriteData.flipVertical) {
		scaling.y *= -1;
		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);
		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, nullptr, 0.0f, &scaling, &spriteCenter, spriteData.angle, &translate);

	IFTHROWFAILED("SetTransform", m_Sprite->SetTransform(&mat));
	IFTHROWFAILED("Draw", m_Sprite->Draw(spriteData.texture, &spriteData.rect, nullptr, nullptr, color));
}

void D3D9GameEngine::startup() {

}

void D3D9GameEngine::build() {
	IFTHROWFAILED("D3D9GameEngine::build - D3DXCreateSprite", D3DXCreateSprite(m_Device.Get(), &m_Sprite));

	UINT width, height;
	LPDIRECT3DTEXTURE9 texture;
	LoadTexture("characters.png", D3DCOLOR_ARGB(0, 255, 255, 255), width, height, texture);

	LONG new_width = (LONG)(width / 23.0f);
	LONG new_height = (LONG)(height / 4.0f);

	spData.scale = 10.0f;
	spData.x = (m_ScreenWidth - new_width * spData.scale) / 2.0f;
	spData.y = (m_ScreenHeight - new_height * spData.scale) / 2.0f;
	spData.width = width;
	spData.height = height;
	spData.texture = texture;
	spData.flipVertical = false;
	spData.flipHorizontal = false;
	spData.angle = 0.0f;
	spData.rect = { 0, 0, new_width, new_height };
}

void D3D9GameEngine::render(float d) {
	m_Device->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	if (SUCCEEDED(m_Device->BeginScene())) {
		m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
		drawSprite(spData, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_Sprite->End();

		m_Device->EndScene();
	}

	m_Device->Present(nullptr, nullptr, nullptr, nullptr);
}