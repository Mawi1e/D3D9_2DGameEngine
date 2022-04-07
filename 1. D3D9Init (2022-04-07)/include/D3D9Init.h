#pragma once

#include "D3D9App.h"

class D3D9Init : public D3D9App {
public:
	virtual void render() override;
	virtual void startup() override;
	virtual void build() override;
};