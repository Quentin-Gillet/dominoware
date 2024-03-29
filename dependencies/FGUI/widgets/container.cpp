//
// FGUI - feature rich graphical user interface
//

// library includes
#include "container.hpp"
#include "../../../base/utilities/logging.h"
#include "../../../base/core/interfaces.h"

namespace FGUI
{
	CContainer::CContainer()
	{
		m_strTitle = "";
		m_strTooltip = "";
		m_bScrollBarState = false;
		m_bIsOpened = false;
		m_uiKey = 0;
		m_anyFont = 0;
		m_iWidgetScrollOffset = 0;
		m_fnctCallback = nullptr;
		m_bIsFocusingOnWidget = false;
		m_pParentWidget = nullptr;
		m_pFocusedWidget = nullptr;
		m_nType = static_cast<int>(WIDGET_TYPE::CONTAINER);
		m_nFlags = static_cast<int>(WIDGET_FLAG::DRAWABLE) | static_cast<int>(WIDGET_FLAG::DRAW_FIRST);
		bIsDraggingThumb = false;
		m_iLastYPosition = 15;
	}

	void CContainer::Render()
	{
		// listen for input
		FGUI::INPUT.PullInput();

		if (!GetParentWidget())
		{
			if (FGUI::INPUT.IsKeyPressed(GetKey()))
			{
				SetState(!GetState());
			}

			if (GetState())
			{
				Update();
				Geometry(FGUI::WIDGET_STATUS::NONE);
				Tooltip();
			}
		}
	}

	void CContainer::SetAccentColor(FGUI::COLOR color)
	{
		if (!GetParentWidget())
			m_accentColor = color;
	}

	FGUI::COLOR CContainer::GetAccentColor()
	{
		if (!GetParentWidget())
			return m_accentColor;
		else
			return {};
	}

	void CContainer::SaveToFile(std::string file)
	{
		nlohmann::json jsModule;

		if (m_prgpWidgets.empty())
		{
			return;
		}

		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			// save widget state
			pWidgets->Save(jsModule);
		}

		std::ofstream ofsFileToSave(file);

		if (ofsFileToSave.fail())
		{
			L::PushConsoleColor(FOREGROUND_RED);
			L::Print("[Config] cannot save config {}", file.c_str());
			L::PopConsoleColor();
			return; // TODO: handle this properly
		}

		// write the file
		ofsFileToSave << std::setw(4) << jsModule << std::endl;
		I::ConVar->ConsoleColorPrintf(Color(m_accentColor.m_ucRed, m_accentColor.m_ucGreen, m_accentColor.m_ucBlue), "[dominoware] config saved.\n");
	}

	void CContainer::LoadFromFile(std::string file)
	{
		nlohmann::json jsModule;

		if (m_prgpWidgets.empty())
		{
			return;
		}

		std::ifstream ifsFileToLoad(file, std::ifstream::binary);

		if (ifsFileToLoad.fail())
		{
			L::PushConsoleColor(FOREGROUND_RED);
			L::Print("[Config] cannot load config {}", file.c_str());
			L::PopConsoleColor();
			return; // TODO: handle this properly
		}

		jsModule = nlohmann::json::parse(ifsFileToLoad);

		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			pWidgets->Load(jsModule);
		}
		I::ConVar->ConsoleColorPrintf(Color(m_accentColor.m_ucRed, m_accentColor.m_ucGreen, m_accentColor.m_ucBlue), "[dominoware] config loaded.\n");
	}

	void CContainer::SetState(bool state)
	{
		m_bIsOpened = state;
	}

	bool CContainer::GetState()
	{
		return m_bIsOpened;
	}

	void CContainer::SetScrollBarState(bool state)
	{
		m_bScrollBarState = state;
	}

	bool CContainer::GetScrollBarState()
	{
		return m_bScrollBarState;
	}

	int CContainer::GetScrollOffset()
	{
		return m_iWidgetScrollOffset;
	}

	void CContainer::SetFocusedWidget(std::shared_ptr<FGUI::CWidgets> widget)
	{
		std::shared_ptr<FGUI::CContainer> cWindow = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer());
		if (widget)
		{
			cWindow->m_pFocusedWidget = widget;

			if (cWindow->m_pFocusedWidget)
			{
				cWindow->m_bIsFocusingOnWidget = true;
			}
		}
		else
		{
			cWindow->m_pFocusedWidget.reset();

			if (!cWindow->m_pFocusedWidget)
			{
				cWindow->m_bIsFocusingOnWidget = false;
			}
		}
	}

	std::shared_ptr<FGUI::CWidgets> CContainer::GetFocusedWidget()
	{
		std::shared_ptr<FGUI::CContainer> cWindow = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer());
		return cWindow->m_pFocusedWidget;
	}

	void CContainer::AddCallback(std::function<void()> callback)
	{
		m_fnctCallback = callback;
	}

	void CContainer::SetKey(unsigned int key)
	{
		m_uiKey = key;
	}

	unsigned int CContainer::GetKey()
	{
		return m_uiKey;
	}

	void CContainer::AddWidget(std::shared_ptr<FGUI::CWidgets> widget, bool padding)
	{
		// set the parent widget
		widget->SetParentWidget(shared_from_this());
		widget->m_anyFont = GetFont();

		// configure padding
		if (padding)
		{
			static constexpr int iScrollBarWidth = 15;
			static constexpr int iWidgetSpace = 12;

			if (GetParentWidget())
			{
				if (widget->GetType() == static_cast<int>(WIDGET_TYPE::COLORPICKER) || widget->GetType() == static_cast<int>(WIDGET_TYPE::KEYBINDER))
				{
					if(m_bScrollBarState)
						widget->SetPosition(GetSize().m_iWidth - widget->GetSizeDefault().m_iWidth - 20, m_iLastYPosition - 3);
					else
						widget->SetPosition(GetSize().m_iWidth - widget->GetSizeDefault().m_iWidth - 10, m_iLastYPosition - 3);
				}
				else if(widget->GetType() == static_cast<int>(WIDGET_TYPE::CHECKBOX) || widget->GetType() == static_cast<int>(WIDGET_TYPE::BUTTON))
				{
					widget->SetPosition(10, m_iLastYPosition);
					m_iLastYPosition += iWidgetSpace + widget->GetSize().m_iHeight;
				}
				else
				{
					widget->SetPosition(10, m_iLastYPosition + 10);
					m_iLastYPosition += iWidgetSpace + widget->GetSize().m_iHeight + 10;
				}

				if (m_bScrollBarState)
				{
					widget->SetSize(m_dmSize.m_iWidth - (widget->GetPosition().m_iX * 2) - iScrollBarWidth, widget->GetSize().m_iHeight);
				}
				else
				{
					widget->SetSize(m_dmSize.m_iWidth - (widget->GetPosition().m_iX * 2), widget->GetSize().m_iHeight);
				}
			}
			else
			{
				widget->SetSize(m_dmSize.m_iWidth - (widget->GetPosition().m_iX * 2), widget->GetSize().m_iHeight);
			}
		}

		// pupulate container
		m_prgpWidgets.emplace_back(widget);

		// re-sort widgets
		if (widget->GetFlags(WIDGET_FLAG::DRAW_FIRST) && widget->GetType() == static_cast<int>(WIDGET_TYPE::CONTAINER))
		{
			// check if the widget is a groupbox Container
			if (widget->GetParentWidget())
			{
				m_prgpWidgets.erase(std::remove(m_prgpWidgets.begin(), m_prgpWidgets.end(), widget), m_prgpWidgets.end());
				m_prgpWidgets.insert(m_prgpWidgets.begin(), widget);
			}
		}
	}

	void CContainer::Geometry(FGUI::WIDGET_STATUS status)
	{
		FGUI::AREA arWidgetRegion = { GetAbsolutePosition().m_iX, GetAbsolutePosition().m_iY, m_dmSize.m_iWidth, m_dmSize.m_iHeight };

		FGUI::DIMENSION dmTitleTextSize = FGUI::RENDER.GetTextSize(m_anyFont, m_strTitle);

		// if the container doesn't have a parent widget, it will behave like a normal window
		if (!GetParentWidget())
		{
			// container body
			FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop + m_dmSize.m_iHeight - 70, arWidgetRegion.m_iRight, 70, { 45, 45, 45 });
			FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, arWidgetRegion.m_iRight, arWidgetRegion.m_iBottom - 70, { 23, 23, 23 });

			// container title
			FGUI::RENDER.Text(m_ptPosition.m_iX + 10, m_ptPosition.m_iY + 10, m_anyFont, { 255, 255, 255 }, m_strTitle);

			// if the container has a function
			if (m_fnctCallback)
			{
				// invoke function
				m_fnctCallback();
			}
		}
		else // otherwise, behave like a normal groupbox
		{
			// groupbox body
			if (m_strTitle.length() > 0)
			{
				//FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, 5, 1, { 55, 55, 55 });                                                                                           // top1
				//FGUI::RENDER.Rectangle((arWidgetRegion.m_iLeft + dmTitleTextSize.m_iWidth) + 10, arWidgetRegion.m_iTop, (arWidgetRegion.m_iRight - dmTitleTextSize.m_iWidth) - 10, 1, { 55, 55, 55 }); // top2
			}
			else
			{
				//FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, arWidgetRegion.m_iRight, 1, { 55, 55, 55 }); // top1
			}

			//FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, 1, arWidgetRegion.m_iBottom, { 55, 55, 55 });                                               // left
			//FGUI::RENDER.Rectangle((arWidgetRegion.m_iLeft + arWidgetRegion.m_iRight), arWidgetRegion.m_iTop, 1, arWidgetRegion.m_iBottom, { 55, 55, 55 });                   // right
			//FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, (arWidgetRegion.m_iTop + arWidgetRegion.m_iBottom), arWidgetRegion.m_iRight, 1, { 55, 55, 55 });                   // bottom
			FGUI::RENDER.Rectangle((arWidgetRegion.m_iLeft + 1), (arWidgetRegion.m_iTop + 1), (arWidgetRegion.m_iRight - 2), (arWidgetRegion.m_iBottom - 2), { 35, 35, 35 }); // background

			// groupbox label
			if (m_strTitle.length() > 0)
			{
				FGUI::RENDER.Text((arWidgetRegion.m_iLeft + 10), arWidgetRegion.m_iTop - (dmTitleTextSize.m_iHeight / 2), m_anyFont, { 255, 255 ,255 }, m_strTitle);
			}

			if (m_bScrollBarState)
			{
				FGUI::AREA arScrollBarRegion = { (arWidgetRegion.m_iLeft + arWidgetRegion.m_iRight) - 15, arWidgetRegion.m_iTop, 15, m_dmSize.m_iHeight };

				static constexpr FGUI::DIMENSION dmScrollBarThumbWidth = { 8, 5 };

				// scrollbar thumb size
				float flScrollbarThumbSize = ((m_dmSize.m_iHeight - m_prgpWidgets.back()->GetSizeDefault().m_iHeight) /
					static_cast<float>(m_prgpWidgets.back()->GetPosition().m_iY)) *
					static_cast<float>((m_dmSize.m_iHeight - m_prgpWidgets.back()->GetSizeDefault().m_iHeight));

				// calculate the scrollbar thumb position
				float flScrollbarThumbPosition = ((m_dmSize.m_iHeight - 10) - flScrollbarThumbSize) * static_cast<float>(m_iWidgetScrollOffset /
					static_cast<float>((m_prgpWidgets.back()->GetPosition().m_iY + m_prgpWidgets.back()->GetSizeDefault().m_iHeight) - (m_dmSize.m_iHeight - 10)));

				// scrollbar body
				FGUI::RENDER.Rectangle(arScrollBarRegion.m_iLeft, arScrollBarRegion.m_iTop, arScrollBarRegion.m_iRight, arScrollBarRegion.m_iBottom, { 50, 50, 50 });

				// scrollbar thumb
				FGUI::RENDER.Rectangle((arScrollBarRegion.m_iLeft + 4), (arScrollBarRegion.m_iTop + flScrollbarThumbPosition) + 5, dmScrollBarThumbWidth.m_iWidth, flScrollbarThumbSize, { 60, 60, 60 });
			}
		}

		// this will tell the container to skip focused widgets (so it can be drawned after all other widgets)
		bool bSkipWidget = false;

		// this will hold the current skipped widget
		std::shared_ptr<FGUI::CWidgets> pWidgetToSkip = nullptr;
		std::shared_ptr<FGUI::CContainer> cWindow = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer());

		if (cWindow->m_bIsFocusingOnWidget)
		{
			if (cWindow->m_pFocusedWidget)
			{
				// set the widget that will be skipped
				pWidgetToSkip = cWindow->m_pFocusedWidget;

				// tell the container to skip this widget
				bSkipWidget = true;
			}
		}

		// iterate over the rest of the widgets
		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			// if a widget is currently being skipped
			if (bSkipWidget)
			{
				// we don't want to draw the skipped widget here
				if (pWidgetToSkip == pWidgets)
				{
					continue;
				}
			}

			// check if widgets are unlocked
			if (pWidgets && pWidgets->IsUnlocked() && pWidgets->GetFlags(WIDGET_FLAG::DRAWABLE))
			{
				// tell if the widget is being hovered or not
				FGUI::WIDGET_STATUS wsWidgetStatus = FGUI::WIDGET_STATUS::NONE;

				FGUI::AREA arWidgetsRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, pWidgets->GetSize().m_iWidth, pWidgets->GetSize().m_iHeight };

				if (m_bScrollBarState)
				{
					// check if the widgets are inside the boundaries of the groupbox
					if ((pWidgets->GetAbsolutePosition().m_iY + pWidgets->GetSizeDefault().m_iHeight) <= (GetAbsolutePosition().m_iY + GetSize().m_iHeight) && (pWidgets->GetAbsolutePosition().m_iY >= GetAbsolutePosition().m_iY))
					{
						if (FGUI::INPUT.IsCursorInArea(arWidgetsRegion) && !GetFocusedWidget())
						{
							wsWidgetStatus = FGUI::WIDGET_STATUS::HOVERED;
						}

						pWidgets->Geometry(wsWidgetStatus);
					}
				}
				else
				{
					if (FGUI::INPUT.IsCursorInArea(arWidgetsRegion) && !GetFocusedWidget())
					{
						wsWidgetStatus = FGUI::WIDGET_STATUS::HOVERED;
					}

					pWidgets->Geometry(wsWidgetStatus);
				}
			}
		}

		// now the container can draw skipped widgets
		if (bSkipWidget)
		{
			// check if the skipped widget can be drawned
			if (pWidgetToSkip && pWidgetToSkip->IsUnlocked() && pWidgetToSkip->GetFlags(WIDGET_FLAG::DRAWABLE))
			{
				// tell if the widget is being hovered or not
				FGUI::WIDGET_STATUS wsSkippedWidgetStatus = FGUI::WIDGET_STATUS::NONE;

				FGUI::AREA arSkippedWidgetRegion = { pWidgetToSkip->GetAbsolutePosition().m_iX, pWidgetToSkip->GetAbsolutePosition().m_iY, pWidgetToSkip->GetSize().m_iWidth, pWidgetToSkip->GetSize().m_iHeight };

				if (m_bScrollBarState)
				{
					// check if the widgets are inside the boundaries of the groupbox
					if ((pWidgetToSkip->GetAbsolutePosition().m_iY + pWidgetToSkip->GetSizeDefault().m_iHeight) <= (GetAbsolutePosition().m_iY + GetSize().m_iHeight) && (pWidgetToSkip->GetAbsolutePosition().m_iY >= GetAbsolutePosition().m_iY))
					{
						if (FGUI::INPUT.IsCursorInArea(arSkippedWidgetRegion))
						{
							wsSkippedWidgetStatus = FGUI::WIDGET_STATUS::HOVERED;
						}

						pWidgetToSkip->Geometry(wsSkippedWidgetStatus);
					}
				}
				else
				{
					if (FGUI::INPUT.IsCursorInArea(arSkippedWidgetRegion))
					{
						wsSkippedWidgetStatus = FGUI::WIDGET_STATUS::HOVERED;
					}

					pWidgetToSkip->Geometry(wsSkippedWidgetStatus);
				}
			}
		}

		IGNORE_ARGS(status);
	}

	void CContainer::Update()
	{
		static std::shared_ptr<FGUI::CContainer> cWindow = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer());
		// check if the container is behaving like a window
		if (!GetParentWidget())
		{
			if (GetFlags(WIDGET_FLAG::FULLSCREEN))
			{
				// change container size
				SetSize(FGUI::RENDER.GetScreenSize());
			}

			FGUI::AREA arDraggableArea = { m_ptPosition.m_iX, m_ptPosition.m_iY, m_dmSize.m_iWidth, 30 };

			static bool bIsDraggingContainer = false;

			if (FGUI::INPUT.IsCursorInArea(arDraggableArea) && !bIsDraggingThumb)
			{
				if (FGUI::INPUT.IsKeyHeld(MOUSE_1))
				{
					bIsDraggingContainer = true;
				}
			}

			// if the user started dragging the container
			if (bIsDraggingContainer)
			{
				FGUI::POINT ptCursorPosDelta = FGUI::INPUT.GetCursorPosDelta();

				// move container
				m_ptPosition.m_iX += ptCursorPosDelta.m_iX;
				m_ptPosition.m_iY += ptCursorPosDelta.m_iY;

				if (GetFlags(WIDGET_FLAG::LIMIT))
				{
					m_ptPosition.m_iX = std::clamp(m_ptPosition.m_iX, 0, (FGUI::RENDER.GetScreenSize().m_iWidth - m_dmSize.m_iWidth));
					m_ptPosition.m_iY = std::clamp(m_ptPosition.m_iY, 0, (FGUI::RENDER.GetScreenSize().m_iHeight - m_dmSize.m_iHeight));
				}

			}

			if (FGUI::INPUT.IsKeyReleased(MOUSE_1))
			{
				bIsDraggingContainer = false;
			}
		}
		else // if the container is behaving like a groupbox
		{
			if (m_bScrollBarState && !cWindow->m_bIsFocusingOnWidget)
			{
				FGUI::AREA arWidgetRegion = { GetAbsolutePosition().m_iX, GetAbsolutePosition().m_iY, m_dmSize.m_iWidth, m_dmSize.m_iHeight };
				FGUI::AREA arScrollBarRegion = { (arWidgetRegion.m_iLeft + arWidgetRegion.m_iRight) - 15, arWidgetRegion.m_iTop, 15, arWidgetRegion.m_iBottom };
				std::shared_ptr<FGUI::CContainer> cWindow = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer());
				static constexpr int iLinesToScroll = 2; // NOTE: feel free to change this

				if (FGUI::INPUT.IsCursorInArea(arScrollBarRegion))
				{
					if (FGUI::INPUT.IsKeyPressed(MOUSE_1))
					{
						bIsDraggingThumb = true;
						cWindow->bIsDraggingThumb = true;
					}
				}

				if (bIsDraggingThumb)
				{
					FGUI::POINT ptCursorPosDelta = FGUI::INPUT.GetCursorPosDelta();

					if (FGUI::INPUT.IsKeyHeld(MOUSE_1))
					{
						m_iWidgetScrollOffset += (ptCursorPosDelta.m_iY * iLinesToScroll);
					}
					else
					{
						bIsDraggingThumb = false;
						cWindow->bIsDraggingThumb = false;
					}

					// disable scrolling if a widget is being focused
					if (GetFocusedWidget())
					{
						bIsDraggingThumb = false;
						cWindow->bIsDraggingThumb = false;
					}
				}

				if (FGUI::INPUT.IsCursorInArea(arWidgetRegion))
				{
					if (FGUI::INPUT.GetMouseScrollWheel(true))
					{
						m_iWidgetScrollOffset += iLinesToScroll * 5;
					}
					else if (FGUI::INPUT.GetMouseScrollWheel(false))
						m_iWidgetScrollOffset -= iLinesToScroll * 5;
				}

				// clamp scrolling
				m_iWidgetScrollOffset = std::clamp(m_iWidgetScrollOffset, 0, std::max(0, ((m_prgpWidgets.back()->GetPosition().m_iY + (m_prgpWidgets.back()->GetSize().m_iHeight + 15))) - m_dmSize.m_iHeight));
			}
		}

		// this will tell the container to skip focused widgets (so it can be drawned after all other widgets)
		bool bSkipWidget = false;

		// this will hold the current skipped widget
		std::shared_ptr<FGUI::CWidgets> pWidgetToSkip = nullptr;

		// handle skipped widgets first
		if (cWindow->m_bIsFocusingOnWidget)
		{
			// check if the skipped widget can be used
			if (cWindow->m_pFocusedWidget && cWindow->m_pFocusedWidget->IsUnlocked())
			{
				// tell the container to skip this widget
				bSkipWidget = true;

				// assign the widget that will be skipped
				pWidgetToSkip = cWindow->m_pFocusedWidget;

				FGUI::AREA arSkippedWidgetRegion = { pWidgetToSkip->GetAbsolutePosition().m_iX, pWidgetToSkip->GetAbsolutePosition().m_iY, pWidgetToSkip->GetSize().m_iWidth, pWidgetToSkip->GetSize().m_iHeight };

				if (m_bScrollBarState)
				{
					if ((pWidgetToSkip->GetAbsolutePosition().m_iY + pWidgetToSkip->GetSizeDefault().m_iHeight) <= (GetAbsolutePosition().m_iY + GetSize().m_iHeight) && (pWidgetToSkip->GetAbsolutePosition().m_iY >= GetAbsolutePosition().m_iY))
					{
						pWidgetToSkip->Update();

						// check if the skipped widget can be clicked
						if (GetFocusedWidget()->GetFlags(WIDGET_FLAG::CLICKABLE) && FGUI::INPUT.IsCursorInArea(arSkippedWidgetRegion) && FGUI::INPUT.IsKeyPressed(MOUSE_1) && bSkipWidget && pWidgetToSkip->GetParentWidget()->GetTitle() == this->GetTitle())
						{
							pWidgetToSkip->Input();

							// loose unfocus
							SetFocusedWidget(nullptr);

							// reset focused widget state
							pWidgetToSkip.reset();
						}
					}
				}
				else
				{
					pWidgetToSkip->Update();

					// check if the skipped widget can be clicked
					if (GetFocusedWidget()->GetFlags(WIDGET_FLAG::CLICKABLE) && FGUI::INPUT.IsCursorInArea(arSkippedWidgetRegion) && FGUI::INPUT.IsKeyPressed(MOUSE_1) && bSkipWidget && pWidgetToSkip->GetParentWidget()->GetTitle() == this->GetTitle())
					{
						pWidgetToSkip->Input();

						// unfocus widget
						SetFocusedWidget(nullptr);

						// reset focused widget state
						pWidgetToSkip.reset();
					}
				}
			}
		}

		// iterate over the rest of the widgets
		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			if (pWidgets->IsUnlocked())
			{
				// if a widget is currently being skipped
				if (bSkipWidget)
				{
					// we don't want to handle skipped widgets here
					if (pWidgetToSkip == pWidgets)
					{
						continue;
					}
				}

				FGUI::AREA arWidgetRegion = {};

				if (m_bScrollBarState)
				{
					if ((pWidgets->GetAbsolutePosition().m_iY + pWidgets->GetSizeDefault().m_iHeight) <= (GetAbsolutePosition().m_iY + GetSize().m_iHeight) && (pWidgets->GetAbsolutePosition().m_iY >= GetAbsolutePosition().m_iY))
					{
						pWidgets->Update();

						// if the widget is a CheckBox allow input if the user clicks on its title
						if (pWidgets->GetType() == static_cast<int>(WIDGET_TYPE::CHECKBOX))
						{
							// get the text size of the CheckBox title
							FGUI::DIMENSION dmCheckBoxTitleTextSize = FGUI::RENDER.GetTextSize(pWidgets->GetFont(), pWidgets->GetTitle());

							arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, (pWidgets->GetSize().m_iWidth + dmCheckBoxTitleTextSize.m_iWidth) + 5, pWidgets->GetSize().m_iHeight };
						}
						else
						{
							arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, pWidgets->GetSize().m_iWidth, pWidgets->GetSize().m_iHeight };
						}

						// check if the widget can be clicked
						if (pWidgets->GetFlags(WIDGET_FLAG::CLICKABLE) && FGUI::INPUT.IsCursorInArea(arWidgetRegion) && FGUI::INPUT.IsKeyPressed(MOUSE_1) && !bSkipWidget)
						{
							pWidgets->Input();

							if (pWidgets->GetFlags(WIDGET_FLAG::FOCUSABLE))
							{
								SetFocusedWidget(pWidgets);
							}
							else
							{
								SetFocusedWidget(nullptr);
							}
						}
					}
				}
				else
				{
					pWidgets->Update();

					// if the widget is a CheckBox allow input if the user clicks on its title
					if (pWidgets->GetType() == static_cast<int>(WIDGET_TYPE::CHECKBOX))
					{
						// get the text size of the CheckBox title
						FGUI::DIMENSION dmCheckBoxTitleTextSize = FGUI::RENDER.GetTextSize(pWidgets->GetFont(), pWidgets->GetTitle());

						arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, (pWidgets->GetSize().m_iWidth + dmCheckBoxTitleTextSize.m_iWidth) + 5, pWidgets->GetSize().m_iHeight };
					}
					else if (pWidgets->GetType() == static_cast<int>(WIDGET_TYPE::TABPANEL))
					{
						std::shared_ptr<FGUI::CTabPanel> cTabs = std::reinterpret_pointer_cast<FGUI::CTabPanel>(pWidgets);
						arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, pWidgets->GetSize().m_iWidth * cTabs->GetTabsCount(), pWidgets->GetSize().m_iHeight};
					}
					else
					{
						arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, pWidgets->GetSize().m_iWidth, pWidgets->GetSize().m_iHeight };
					}

					// check if the widget can be clicked
					if (pWidgets->GetFlags(WIDGET_FLAG::CLICKABLE) && FGUI::INPUT.IsCursorInArea(arWidgetRegion) && FGUI::INPUT.IsKeyPressed(MOUSE_1) && !bSkipWidget)
					{
						pWidgets->Input();

						if (pWidgets->GetFlags(WIDGET_FLAG::FOCUSABLE))
						{
							SetFocusedWidget(pWidgets);
						}
						else
						{
							SetFocusedWidget(nullptr);
						}
					}
				}
			}
		}
	}

	void CContainer::Input()
	{
	}

	void CContainer::Save(nlohmann::json& module)
	{
		if (m_prgpWidgets.empty())
		{
			return;
		}

		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			if (pWidgets->GetType() == static_cast<int>(WIDGET_TYPE::CONTAINER))
			{
				pWidgets->Save(module);
			}
			else if (pWidgets->GetFlags(WIDGET_FLAG::SAVABLE)) // check if the widget can be saved
			{
				pWidgets->Save(module);
			}
		}
	}

	void CContainer::Load(nlohmann::json& module)
	{
		if (m_prgpWidgets.empty())
		{
			return;
		}

		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			if (pWidgets->GetType() == static_cast<int>(WIDGET_TYPE::CONTAINER))
			{
				pWidgets->Load(module);
			}
			else if (pWidgets->GetFlags(WIDGET_FLAG::SAVABLE)) // check if the widget can be loaded
			{
				pWidgets->Load(module);
			}
		}
	}

	void CContainer::Tooltip()
	{
		for (std::shared_ptr<FGUI::CWidgets> pWidgets : m_prgpWidgets)
		{
			// check if widgets are unlocked
			if (pWidgets && pWidgets->IsUnlocked() && pWidgets->GetFlags(WIDGET_FLAG::DRAWABLE))
			{
				// avoid drawing tooltips when a widget is being focused
				if (!std::reinterpret_pointer_cast<FGUI::CContainer>(pWidgets->GetParentWidget())->GetFocusedWidget())
				{
					FGUI::AREA arWidgetRegion = { pWidgets->GetAbsolutePosition().m_iX, pWidgets->GetAbsolutePosition().m_iY, pWidgets->GetSize().m_iWidth, pWidgets->GetSize().m_iHeight };

					if (FGUI::INPUT.IsCursorInArea(arWidgetRegion))
					{
						pWidgets->Tooltip();
					}
				}
			}
		}
	}
} // namespace FGUI