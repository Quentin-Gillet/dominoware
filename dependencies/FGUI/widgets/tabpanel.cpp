//
// FGUI - feature rich graphical user interface
//

// library includes
#include "tabpanel.hpp"
#include "container.hpp"

#include "../../../base/utilities/logging.h"

namespace FGUI
{

	CTabPanel::CTabPanel()
	{
		m_strTitle = "TabsPanel";
		m_anyFont = 0;
		m_ullSelectedEntry = 0;
		m_dmSize = { 110, 25 };
		m_dmSizeDefault = { m_dmSize.m_iWidth, m_dmSize.m_iHeight };
		m_prgpTabButtons = {};
		m_strTooltip = "";
		m_nStyle = static_cast<int>(TAB_STYLE::HORIZONTAL);
		m_nType = static_cast<int>(WIDGET_TYPE::TABPANEL);
		m_nFlags = static_cast<int>(WIDGET_FLAG::DRAWABLE) | static_cast<int>(WIDGET_FLAG::CLICKABLE);
	}

	void CTabPanel::AddTab(std::string title)
	{
		m_prgpTabButtons.emplace_back(title);
	}

	void CTabPanel::SetIndex(std::size_t index)
	{
		m_ullSelectedEntry = index;
	}

	std::size_t CTabPanel::GetIndex()
	{
		return m_ullSelectedEntry;
	}

	void CTabPanel::SetStyle(FGUI::TAB_STYLE style)
	{
		m_nStyle = static_cast<int>(style);
	}

	int CTabPanel::GetStyle()
	{
		return m_nStyle;
	}

	int CTabPanel::GetTabsCount()
	{
		return m_prgpTabButtons.size();
	}

	void CTabPanel::Geometry(FGUI::WIDGET_STATUS status)
	{
		// don't proceed if the tab container is empty
		if (m_prgpTabButtons.empty())
		{
			return;
		}

		FGUI::AREA arWidgetRegion = { 0, 0, 0, 0 };
		FGUI::COLOR accentColor = std::reinterpret_pointer_cast<FGUI::CContainer>(GetWindowContainer())->GetAccentColor();

		/*FGUI::RENDER.Gradient(GetAbsolutePosition().m_iX + 20, GetAbsolutePosition().m_iY + 50, (m_dmSize.m_iWidth * m_prgpTabButtons.size() / 2) - 20, 3, { 45, 45, 45 }, accentColor, true);
		FGUI::RENDER.Gradient(GetAbsolutePosition().m_iX - 20 + (m_dmSize.m_iWidth * m_prgpTabButtons.size() / 2), GetAbsolutePosition().m_iY + 50, (m_dmSize.m_iWidth * m_prgpTabButtons.size() / 2) - 20, 3, accentColor, { 45, 45, 45 }, true);*/

		for (std::size_t i = 0; i < m_prgpTabButtons.size(); i++)
		{
			if (m_nStyle == static_cast<int>(TAB_STYLE::HORIZONTAL))
			{
				m_iEntrySpacing = 120;

				arWidgetRegion = { GetAbsolutePosition().m_iX + (static_cast<int>(i) * m_iEntrySpacing), GetAbsolutePosition().m_iY, m_dmSize.m_iWidth, m_dmSize.m_iHeight };

				FGUI::DIMENSION dimTextSize = FGUI::RENDER.GetTextSize(GetFont(), m_prgpTabButtons[i]);

				if (m_ullSelectedEntry == i)
				{
					FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, (arWidgetRegion.m_iTop - 5), arWidgetRegion.m_iRight, 2, accentColor);
					FGUI::RENDER.Text((arWidgetRegion.m_iLeft + (arWidgetRegion.m_iRight / 2) - (dimTextSize.m_iWidth / 2)), (arWidgetRegion.m_iTop + (arWidgetRegion.m_iBottom / 2) - 5), m_anyFont, { 255, 255, 255 }, m_prgpTabButtons[i]);
				}
				else
				{
					//FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, arWidgetRegion.m_iRight, arWidgetRegion.m_iBottom, { 45, 45, 45 });
					FGUI::RENDER.Text((arWidgetRegion.m_iLeft + (arWidgetRegion.m_iRight / 2) - (dimTextSize.m_iWidth / 2)), (arWidgetRegion.m_iTop + (arWidgetRegion.m_iBottom / 2) - 5), m_anyFont, { 255, 255, 255 }, m_prgpTabButtons[i]);
					if (FGUI::INPUT.IsCursorInArea(arWidgetRegion))
						FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, (arWidgetRegion.m_iTop - 5), arWidgetRegion.m_iRight, 2, { accentColor.m_ucRed, accentColor.m_ucGreen, accentColor.m_ucBlue, 80.f });
				}
			}
			else if (m_nStyle == static_cast<int>(TAB_STYLE::VERTICAL))
			{
				m_iEntrySpacing = 33;

				arWidgetRegion = { GetAbsolutePosition().m_iX, GetAbsolutePosition().m_iY + (static_cast<int>(i) * m_iEntrySpacing), m_dmSize.m_iWidth, m_dmSize.m_iHeight };

				if (m_ullSelectedEntry == i)
				{
					FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, (arWidgetRegion.m_iRight + 5), arWidgetRegion.m_iBottom, { 45, 45, 45 });
					FGUI::RENDER.Text((arWidgetRegion.m_iLeft + 20), (arWidgetRegion.m_iTop + (arWidgetRegion.m_iBottom / 2) - 5), m_anyFont, { 255, 255, 255 }, m_prgpTabButtons[i]);
				}
				else
				{
					FGUI::RENDER.Rectangle(arWidgetRegion.m_iLeft, arWidgetRegion.m_iTop, arWidgetRegion.m_iRight, arWidgetRegion.m_iBottom, { 45, 45, 45 });
					FGUI::RENDER.Text((arWidgetRegion.m_iLeft + 20), (arWidgetRegion.m_iTop + (arWidgetRegion.m_iBottom / 2) - 5), m_anyFont, { 255, 255, 255 }, m_prgpTabButtons[i]);
				}
			}
		}

		IGNORE_ARGS(status);
	}

	void CTabPanel::Update()
	{
	}

	void CTabPanel::Input()
	{
		FGUI::AREA arWidgetRegion = { 0, 0, 0, 0 };

		for (std::size_t i = 0; i < m_prgpTabButtons.size(); i++)
		{
			if (m_nStyle == static_cast<int>(TAB_STYLE::HORIZONTAL))
			{
				m_iEntrySpacing = 113;

				arWidgetRegion = { GetAbsolutePosition().m_iX + (static_cast<int>(i) * m_iEntrySpacing), GetAbsolutePosition().m_iY, m_dmSize.m_iWidth, m_dmSize.m_iHeight };
			}
			else if (m_nStyle == static_cast<int>(TAB_STYLE::VERTICAL))
			{
				m_iEntrySpacing = 33;

				arWidgetRegion = { GetAbsolutePosition().m_iX, GetAbsolutePosition().m_iY + (static_cast<int>(i) * m_iEntrySpacing), m_dmSize.m_iWidth, m_dmSize.m_iHeight };
			}

			if (FGUI::INPUT.IsCursorInArea(arWidgetRegion))
			{
				if (FGUI::INPUT.IsKeyPressed(MOUSE_1))
				{
					m_ullSelectedEntry = i;

					// loose focus if another tab is selected
					if (m_ullSelectedEntry != i)
					{
						std::reinterpret_pointer_cast<FGUI::CContainer>(GetParentWidget())->SetFocusedWidget(nullptr);
					}
				}
			}
		}
	}

	void CTabPanel::Save(nlohmann::json& module)
	{
		IGNORE_ARGS(module);
	}

	void CTabPanel::Load(nlohmann::json& module)
	{
		IGNORE_ARGS(module);
	}

	void CTabPanel::Tooltip()
	{
	}

} // namespace FGUI