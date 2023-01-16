#include "pch.h"
#include "NApplication.h"


NApplication* NApplication::m_pInstance;
Overlay NApplication::m_pOverlay;

NApplication::NApplication()
{
	OutputDebugStringA("NApplication ctor holy");
	m_pInstance = this;
	m_pOverlay = Overlay();
	printf("NApplication ctor\n");
}

NApplication* NApplication::GetInstance(void)
{
	return m_pInstance;
}

Overlay* NApplication::GetOverlay(void)
{
	NApplication* nApp = GetInstance();
	if (nApp == NULL) return NULL;
	return &nApp->m_pOverlay;
}
