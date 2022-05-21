#include "draw.h"

// used: engine interface
#include "../core/interfaces.h"
// used: current window
#include "inputsystem.h"

#pragma region draw_get
void D::Setup()
{
	// setup fonts
	F::CreateSurfaceFont(F::Whitney, "Whitney-Medium", 20, EFontFlags::FONTFLAG_ANTIALIAS, true);

	/*F::Icons = I::Surface->FontCreate();
	I::Surface->SetFontGlyphSet(F::Icons, "qo0_icons", 40, 500, 0, 0, EFontFlags::FONTFLAG_ANTIALIAS);*/

	F::WhitneyMenu.m_strFamily = "Whitney-Medium";
	F::WhitneyMenu.m_iSize = 14;
	F::WhitneyMenu.m_bBold = false;
	F::WhitneyMenu.m_nFlags = EFontFlags::FONTFLAG_ANTIALIAS;

	FGUI::RENDER.CreateFontA = F::CreateSurfaceFont;
	FGUI::RENDER.GetScreenSize = D::GetScreenSize;
	FGUI::RENDER.GetTextSize = D::GetTextSize;
	FGUI::RENDER.Rectangle = D::R::AddRect;
	FGUI::RENDER.Line = D::R::AddLine;
	FGUI::RENDER.Outline = D::R::AddOutlinedRect;
	FGUI::RENDER.Gradient = D::R::AddGradientRect;
	FGUI::RENDER.Rounded = D::R::AddRoundedRect;
	FGUI::RENDER.Text = D::R::AddText;

	L::Print(XorStr("[RENDER] Render initialized."));
}

void F::CreateSurfaceFont(FGUI::FONT& hFont, std::string szFontName, int iSize, int iFlags, bool bBold)
{
	hFont = I::Surface->FontCreate();
	I::Surface->SetFontGlyphSet(hFont, szFontName.c_str(), iSize, bBold ? 500 : 0, 0, 0, iFlags);
}
#pragma endregion

#pragma region draw_render
void D::AddLine(const Vector2D& vecStart, const Vector2D& vecEnd, const Color& colLine)
{
	I::Surface->DrawSetColor(colLine);
	I::Surface->DrawLine(vecStart.x, vecStart.y, vecEnd.x, vecEnd.y);
}

void D::AddText(const Vector2D& vecPosition, const HFont hFont, const std::string& szText, const Color& colText)
{
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, szText.c_str(), -1, temp, 128) > 0)
	{
		I::Surface->DrawSetTextFont(hFont);
		I::Surface->DrawSetTextPos(vecPosition.x, vecPosition.y);
		I::Surface->DrawSetTextColor(colText);

		I::Surface->DrawPrintText(temp, wcslen(temp));
	}
}

void D::AddText(const int x, const int y, const HFont hFont, const std::string& szText, const Color& colText)
{
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, szText.c_str(), -1, temp, 128) > 0)
	{
		I::Surface->DrawSetTextFont(hFont);
		I::Surface->DrawSetTextPos(x, y);
		I::Surface->DrawSetTextColor(colText);

		I::Surface->DrawPrintText(temp, wcslen(temp));
	}
}

void D::AddRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawOutlinedRect(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y);
}

void D::AddRect(const int x, const int y, const int w, const int h, const Color& colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void D::AddFilledRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawFilledRect(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y);
}

void D::AddFilledRect(const int x, const int y, const int w, const int h, const Color& colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawFilledRect(x, y, x + w, y + h);
}

void D::AddTexturedPolygon(const std::int32_t iVerticesCount, Vertex_t* verVertices, const Color& colPoly)
{
	static unsigned char ucBuffer[4] = { 255,255,255,255 };
	unsigned int iTextureId{};
	if (!iTextureId)
	{
		iTextureId = I::Surface->CreateNewTextureID(true);
		I::Surface->DrawSetTextureRGBA(iTextureId, ucBuffer, 1, 1);
	}
	I::Surface->DrawSetColor(colPoly);
	I::Surface->DrawSetTexture(iTextureId);
	I::Surface->DrawTexturedPolygon(iVerticesCount, verVertices);
}

void D::AddCircle(const Vector2D& vecPosition, const std::int32_t iRadius, const std::int32_t iSegments, const Color& colCircle)
{
	float flStep = M_PI * 2.0 / iSegments;
	for (float a = 0; a < (M_PI * 2.0); a += flStep) {
		float x1 = iRadius * cos(a) + vecPosition.x;
		float y1 = iRadius * sin(a) + vecPosition.y;
		float x2 = iRadius * cos(a + flStep) + vecPosition.x;
		float y2 = iRadius * sin(a + flStep) + vecPosition.y;
		I::Surface->DrawSetColor(colCircle);
		I::Surface->DrawLine(x1, y1, x2, y2);
	}
}

void D::AddVerticalGradientRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRectTop, const Color& colRectBottom)
{
	I::Surface->DrawSetColor(colRectTop);
	I::Surface->DrawFilledRectFade(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y, 255, 255, false);
	I::Surface->DrawSetColor(colRectBottom);
	I::Surface->DrawFilledRectFade(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y, 0, 255, false);
}

void D::AddHorizontalGradientRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRectLeft, const Color& colRectRight)
{
	I::Surface->DrawSetColor(colRectLeft);
	I::Surface->DrawFilledRectFade(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y, 255, 255, true);
	I::Surface->DrawSetColor(colRectRight);
	I::Surface->DrawFilledRectFade(vecPosition.x, vecPosition.y, vecPosition.x + vecSize.x, vecPosition.y + vecSize.y, 0, 255, true);
}

void D::AddHorizontalGradientRect(const int x, const int y, const int w, const int h, const Color& colRectLeft, const Color& colRectRight)
{
	I::Surface->DrawSetColor(colRectLeft);
	I::Surface->DrawFilledRectFade(x, y, x + w, y + h, 255, 255, true);
	I::Surface->DrawSetColor(colRectRight);
	I::Surface->DrawFilledRectFade(x, y, x + w, y + h, 0, 255, true);
}

void D::AddRoundedRect(const int iX, const int iY, const int iWidth, const int iHeight, const Color& colRect, const int iRadius)
{
	Vertex_t round[64];

	for (int i = 0; i < 4; i++)
	{
		int _x = iX + ((i < 2) ? (iWidth - iRadius) : iRadius);
		int _y = iY + ((i % 3) ? (iHeight - iRadius) : iRadius);

		float a = 90.f * i;

		for (int j = 0; j < 16; j++)
		{
			float _a = M_DEG2RAD(a + j * 6.f);

			round[(i * 16) + j] = Vertex_t(Vector2D(_x + iRadius * sin(_a), _y - iRadius * cos(_a)));
		}
	}

	D::AddTexturedPolygon(64, round, colRect);
}
#pragma endregion

#pragma region draw_extra
bool D::WorldToScreen(const Vector& vecOrigin, Vector2D& vecScreen)
{
	const ViewMatrix_t& matWorldToScreen = I::Engine->WorldToScreenMatrix();
	const float flWidth = matWorldToScreen[3][0] * vecOrigin.x + matWorldToScreen[3][1] * vecOrigin.y + matWorldToScreen[3][2] * vecOrigin.z + matWorldToScreen[3][3];

	if (flWidth < 0.001f)
		return false;

	// compute the scene coordinates of a point in 3d
	const float flInverse = 1.0f / flWidth;
	vecScreen.x = (matWorldToScreen[0][0] * vecOrigin.x + matWorldToScreen[0][1] * vecOrigin.y + matWorldToScreen[0][2] * vecOrigin.z + matWorldToScreen[0][3]) * flInverse;
	vecScreen.y = (matWorldToScreen[1][0] * vecOrigin.x + matWorldToScreen[1][1] * vecOrigin.y + matWorldToScreen[1][2] * vecOrigin.z + matWorldToScreen[1][3]) * flInverse;

	// screen transform
	// get the screen position in pixels of given point
	int x, y;
	I::Engine->GetScreenSize(x, y);

	const Vector2D vecDisplaySize = Vector2D(x, y);
	vecScreen.x = (vecDisplaySize.x * 0.5f) + (vecScreen.x * vecDisplaySize.x) * 0.5f;
	vecScreen.y = (vecDisplaySize.y * 0.5f) - (vecScreen.y * vecDisplaySize.y) * 0.5f;
	return true;
}

FGUI::DIMENSION D::GetTextSize(FGUI::FONT hFont, std::string szText)
{
	std::wstring wsTemp(szText.begin(), szText.end());
	const wchar_t* wstr = wsTemp.c_str();
	FGUI::DIMENSION dimTextSize = { 0, 0 };

	I::Surface->GetTextSize(hFont, wstr, dimTextSize.m_iWidth, dimTextSize.m_iHeight);
	return dimTextSize;
}

Vector2D D::GetTextSize(const HFont hFont, const std::string& szText)
{
	std::wstring wsTemp(szText.begin(), szText.end());
	const wchar_t* wstr = wsTemp.c_str();
	FGUI::DIMENSION dimTextSize = { 0, 0 };
	static int w, h;

	I::Surface->GetTextSize(hFont, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

FGUI::DIMENSION D::GetScreenSize()
{
	static FGUI::DIMENSION dimSize = { 0, 0 };
	I::Engine->GetScreenSize(dimSize.m_iWidth, dimSize.m_iHeight);

	return dimSize;
}
#pragma endregion

#pragma region fgui renderer

void D::R::AddLine(const int iStarX, const int iStartY, const int iEndX, const int iEndY, FGUI::COLOR colLine)
{
	I::Surface->DrawSetColor(colLine);
	I::Surface->DrawLine(iStarX, iStartY, iEndX, iEndY);
}

void D::R::AddText(const int iX, const int iY, FGUI::FONT hFont, FGUI::COLOR colText, std::string szText)
{
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, szText.c_str(), -1, temp, 128) > 0)
	{
		I::Surface->DrawSetTextFont(hFont);
		I::Surface->DrawSetTextPos(iX, iY);
		I::Surface->DrawSetTextColor(colText);

		I::Surface->DrawPrintText(temp, wcslen(temp));
	}
}

void D::R::AddOutlinedRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawOutlinedRect(iX, iY, iX + iWidth, iY + iHeight);
}

void D::R::AddRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRect)
{
	I::Surface->DrawSetColor(colRect);
	I::Surface->DrawFilledRect(iX, iY, iX + iWidth, iY + iHeight);
}

void D::R::AddTexturedPolygon(const std::int32_t iVerticesCount, Vertex_t* verVertices, FGUI::COLOR colPoly)
{
	static unsigned char ucBuffer[4] = { 255,255,255,255 };
	unsigned int iTextureId{};
	if (!iTextureId)
	{
		iTextureId = I::Surface->CreateNewTextureID(true);
		I::Surface->DrawSetTextureRGBA(iTextureId, ucBuffer, 1, 1);
	}
	I::Surface->DrawSetColor(colPoly);
	I::Surface->DrawSetTexture(iTextureId);
	I::Surface->DrawTexturedPolygon(iVerticesCount, verVertices);
}

void D::R::AddGradientRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRectLeft, FGUI::COLOR colRectRight, bool bHorizontal)
{
	I::Surface->DrawSetColor(colRectLeft);
	I::Surface->DrawFilledRectFade(iX, iY, iX + iWidth, iY + iHeight, 255, 255, bHorizontal);
	I::Surface->DrawSetColor(colRectRight);
	I::Surface->DrawFilledRectFade(iX, iY, iX + iWidth, iY + iHeight, 0, 255, bHorizontal);
}

void D::R::AddRoundedRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRect, const int iRadius)
{
	Vertex_t round[64];

	for (int i = 0; i < 4; i++)
	{
		int _x = iX + ((i < 2) ? (iWidth - iRadius) : iRadius);
		int _y = iY + ((i % 3) ? (iHeight - iRadius) : iRadius);

		float a = 90.f * i;

		for (int j = 0; j < 16; j++)
		{
			float _a = M_DEG2RAD(a + j * 6.f);

			round[(i * 16) + j] = Vertex_t(Vector2D(_x + iRadius * sin(_a), _y - iRadius * cos(_a)));
		}
	}

	D::R::AddTexturedPolygon(64, round, colRect);
}

#pragma endregion
