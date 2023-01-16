#pragma once
#include "Overlay.h"

class NApplication
{
private:
	static NApplication* m_pInstance;
	static Overlay m_pOverlay;

public:
	NApplication();

	/*--------------------*/
	// Global getters setup
	static NApplication* GetInstance();
	static Overlay* GetOverlay();

	// I'm lazy
	uintptr_t clientBase = (uintptr_t)GetModuleHandleA("rs2client.exe");
	uintptr_t openglBase = (uintptr_t)GetModuleHandleA("opengl32.dll");
	std::string TITLE_TEXT = "NXTLite - github.com/zkxjzmswkwl/NXTLite";
};

