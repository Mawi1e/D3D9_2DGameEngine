#pragma once

#include "D3D9App.h"

struct SpriteData {
	int width, height;
	float x, y;
	float angle;
	float scale;
	RECT rect;
	LPDIRECT3DTEXTURE9 texture;
	bool flipHorizontal;
	bool flipVertical;
};

class D3D9GameEngine : public D3D9App {
public:
	void LoadTexture(const char* filepath,
		COLOR_ARGB transcolor, UINT& width, UINT& height, LPDIRECT3DTEXTURE9& texture);
	void drawSprite(SpriteData& spriteData, COLOR_ARGB color);

public:
	virtual void render(float d) override;
	virtual void startup() override;
	virtual void build() override;

private:
	SpriteData spData;

};