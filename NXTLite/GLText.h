#pragma once
#include <Windows.h>
#include <stdio.h>
#include <gl/GL.h>

// for glPrint()
HDC				hDC;
HFONT			hOldFont;
HFONT			hFont;
UINT			FontBase, FontBaseUnderline, FontBaseBig;
bool                     bFontsBuild = 0;

// ----------------------------------------------------------------------

void BuildFonts()
{
	hDC = wglGetCurrentDC();

	FontBase = glGenLists(96);
	hFont = CreateFont(-12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Ebrima"); //"Lucida Console");					
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 32, 96, FontBase);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	FontBaseUnderline = glGenLists(96);
	hFont = CreateFont(-10, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Ebrima");
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 32, 96, FontBaseUnderline);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	FontBaseBig = glGenLists(96);
	hFont = CreateFont(-14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 32, 96, FontBaseBig);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);

	bFontsBuild = true;
}

// ----------------------------------------------------------------------

void glPrint(int x, int y, const char* fmt, ...)
{
	if (!bFontsBuild) BuildFonts();

	if (fmt == NULL)	return;

	glRasterPos2i(x, y);

	char		text[256];
	va_list		ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(FontBase - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

// ----------------------------------------------------------------------

void glPrintUnderline(int x, int y, const char* fmt, ...)
{
	if (!bFontsBuild) BuildFonts();

	if (fmt == NULL)	return;

	glRasterPos2i(x, y);

	char		text[256];
	va_list		ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(FontBaseUnderline - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

// ----------------------------------------------------------------------

void glPrintBig(int x, int y, const char* fmt, ...)
{
	if (!bFontsBuild) BuildFonts();

	if (fmt == NULL)	return;

	glRasterPos2i(x, y);

	char		text[256];
	va_list		ap;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(FontBaseBig - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}