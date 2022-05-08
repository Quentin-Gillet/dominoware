#pragma once
#pragma once
// used: winapi, directx, imgui, fmt includes
#include "../common.h"
// used: Color
#include "../sdk/datatypes/Color.h"
// used: vector
#include "../sdk/datatypes/vector.h"
// used: M_PI
#include "math.h"
// used: iSurface
#include "../sdk/interfaces/isurface.h"
// used: logging
#include "logging.h"


/*
 * FONTS
 */
namespace F
{
	/*
	 * Fonts navigation:
	 * [N][purpose]	[max size]	[flags]
	 */

	// 0	main		13		autohinting
	inline FGUI::FONT Whitney;
	inline FGUI::WIDGET_FONT WhitneyMenu;
	// 1	extra		14		bold
	//inline ImFont* Whitney;
	// 2	visuals		40		lighthinting
	//inline ImFont* SmallestPixel;
	// 3	icons		40		lighthinting
	inline FGUI::FONT Icons;

	void CreateSurfaceFont(FGUI::FONT& hFont, std::string szFontName, int iSize, int iFlags, bool bBold);
}

/*
 * DRAW
 * ready rendering framework
 */
namespace D
{
	// Get
	/* create Fonts, set styles etc */
	void Setup();

	// Default Render
	void AddLine(const Vector2D& vecStart, const Vector2D& vecEnd, const Color& colLine);
	void AddText(const Vector2D& vecPosition, const HFont hFont, const std::string& szText, const Color& colText);
	void AddText(const int x, const int y, const HFont hFont, const std::string& szText, const Color& colText);
	void AddRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRect);
	void AddRect(const int x, const int y, const int w, const int h, const Color& colRect);
	void AddFilledRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRect);
	void AddFilledRect(const int x, const int y, const int w, const int h, const Color& colRect);
	void AddTexturedPolygon(const std::int32_t iVerticesCount, Vertex_t* verVertices, const Color& colPoly);
	void AddCircle(const Vector2D& vecPosition, const std::int32_t iRadius, const std::int32_t iSegments, const Color& colCircle);
	void AddVerticalGradientRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRectTop, const Color& colRectBottom);
	void AddHorizontalGradientRect(const Vector2D& vecPosition, const Vector2D& vecSize, const Color& colRectLeft, const Color& colRectRight);
	void AddHorizontalGradientRect(const int x, const int y, const int w, const int h, const Color& colRectLeft, const Color& colRectRight);

	namespace R
	{
		void AddLine(const int iStarX, const int iStartY, const int iEndX, const int iEndY, FGUI::COLOR colLine);
		void AddText(const int iX, const int iY, FGUI::FONT hFont, FGUI::COLOR colText, std::string szText);
		void AddOutlinedRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRect);
		void AddRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRect);
		void AddTexturedPolygon(const std::int32_t iVerticesCount, Vertex_t* verVertices, FGUI::COLOR colPoly); //TO IMPLEMENTS
		void AddGradientRect(const int iX, const int iY, const int iWidth, const int iHeight, FGUI::COLOR colRectLeft, FGUI::COLOR colRectRight, bool bHorizontal);
		void AddRoundedRect(const int iX, const int iY, const int iWidth, const int iHeight, const int iRadius, FGUI::COLOR colRect);
	}

	// Extra
	/* converts 3d game world space to screen space */
	bool WorldToScreen(const Vector& vecOrigin, Vector2D& vecScreen);
	/* get Text size */
	Vector2D GetTextSize(const HFont hFont, const std::string& szText);
	FGUI::DIMENSION GetTextSize(FGUI::FONT hFont, std::string szText);
	// get Screen size
	FGUI::DIMENSION GetScreenSize();
}
