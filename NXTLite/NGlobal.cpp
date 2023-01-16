#include "pch.h"
#include "NGlobal.h"
#include "NApplication.h"

NApplication* NGetApplication(void)
{
	return NApplication::GetInstance();
}

Overlay* NGetOverlay(void)
{
	return NApplication::GetOverlay();
}
