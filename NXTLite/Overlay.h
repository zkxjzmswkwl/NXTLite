#pragma once
#include "pch.h"
#include "MinHook.h"
#include "Skills.h"
#include "NApplication.h"


class Overlay
{
private:

	std::string m_fontPath = "C:/Users/Carter/Desktop/Ruda-Bold.ttf";
	float m_fontSize = 24.000f;
	// TODO: Fix this laziness.
	SkillTree* skillTree = nullptr;

public:
	Overlay();
	~Overlay();
	void InitStyle();
	void StatTrackWindow();
};