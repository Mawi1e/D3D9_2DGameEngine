#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <Xinput.h>
#include <dinput.h>

namespace KeyNS {
	const int KEY_ARRAY_LEN = 256;
}

class Input {
public:
	Input();
	Input& operator=(const Input&) = delete;
	Input(const Input&) = delete;
	~Input();

	void Initialize();

private:


};