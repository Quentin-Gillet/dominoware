#include "menu.h"

void W::Initialize()
{
	static FGUI::CBuilder cBuilder;
	Widgets::cMainMenu = std::make_shared<FGUI::CContainer>();
	Widgets::cMainMenu->SetCursor(FGUI::CURSOR_STYLE::NONE);
	Widgets::cMainMenu->SetAccentColor({ 14 , 180, 191 });

	cBuilder.Widget(Widgets::cMainMenu).Title("Dominoware.xyz").Position(400, 400).Size(650, 400).Font(F::WhitneyMenu).Key(S::KEY_INSERT).Flag(FGUI::WIDGET_FLAG::LIMIT);
	{
		Widgets::cMainTabPanel = std::make_shared<FGUI::CTabPanel>();
		Widgets::cMainTabPanel->SetStyle(FGUI::ESTabLayout_t::HORIZONTAL);
		
		cBuilder.Widget(Widgets::cMainTabPanel).Position(10, 300).Font(F::WhitneyMenu).Tabs({ "LegitBot", "Visuals", "Misc", "Skins", "Config" }).SpawnIn(Widgets::cMainMenu, false);
		{
			Widgets::cGroupbox = std::make_shared<FGUI::CContainer>();
			cBuilder.Widget(Widgets::cGroupbox).Title("Test").Position(10, 20).Size(240, 240).Font(F::WhitneyMenu).Medium(Widgets::cMainTabPanel, 0).SpawnIn(Widgets::cMainMenu, false);
			{
				Widgets::cCheckTest = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cCheckTest).Title("Test Check").Position(10, 20).Font(F::WhitneyMenu).Tooltip("test tooltip").SpawnIn(Widgets::cGroupbox, true);

				Widgets::cColorPicker = std::make_shared<FGUI::CColorPicker>();
				cBuilder.Widget(Widgets::cColorPicker).Title("Accent Color").Position(215, 20).Color(Widgets::cMainMenu->GetAccentColor()).Callback(UpdateAccentColor).SpawnIn(Widgets::cGroupbox);
			}
		}
	}
}

void W::UpdateAccentColor()
{
	W::Widgets::cMainMenu->SetAccentColor(W::Widgets::cColorPicker->GetColor());
	L::Print("CHANGE");
}

void W::Render()
{
	Widgets::cMainMenu->Render();
}