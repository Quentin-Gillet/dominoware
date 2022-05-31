#include "menu.h"

void W::Initialize()
{
	static FGUI::CBuilder cBuilder;
	Widgets::cMainMenu = std::make_shared<FGUI::CContainer>();
	Widgets::cMainMenu->SetAccentColor({ 14 , 180, 191 });

	cBuilder.Widget(Widgets::cMainMenu).Title("Dominoware.xyz").Position(400, 400).Size(650, 400).Font(F::WhitneyMenu).Key(S::KEY_RCONTROL).Flag(FGUI::WIDGET_FLAG::LIMIT);
	{
		Widgets::cMainTabPanel = std::make_shared<FGUI::CTabPanel>();
		Widgets::cMainTabPanel->SetStyle(FGUI::ESTabLayout_t::HORIZONTAL);

		cBuilder.Widget(Widgets::cMainTabPanel).Position(10, 300).Font(F::WhitneyMenu).Tabs({ "LegitBot", "Visuals", "Skins", "Misc", "Config" }).SpawnIn(Widgets::cMainMenu, false);
		{
			Widgets::cGroupbox = std::make_shared<FGUI::CContainer>();
			cBuilder.Widget(Widgets::cGroupbox).Title("Test").Position(10, 20).Size(240, 240).Font(F::WhitneyMenu).Medium(Widgets::cMainTabPanel, 0).ScrollBar(true).SpawnIn(Widgets::cMainMenu, false);
			{
				Widgets::cColorPicker = std::make_shared<FGUI::CColorPicker>();
				cBuilder.Widget(Widgets::cColorPicker).Title("Accent Color").Position(200, 12).Color(Widgets::cMainMenu->GetAccentColor()).Callback(UpdateAccentColor).SpawnIn(Widgets::cGroupbox);

				Widgets::cCheckTest = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cCheckTest).Title("Test Check").Font(F::WhitneyMenu).Tooltip("test tooltip").SpawnIn(Widgets::cGroupbox);

				Widgets::cSlider = std::make_shared<FGUI::CSlider>();
				cBuilder.Widget(Widgets::cSlider).Title("Slider wesh").Font(F::WhitneyMenu).Prefix("%").Range(0, 100).Value(0).SpawnIn(Widgets::cGroupbox);

				Widgets::cComboBox = std::make_shared<FGUI::CComboBox>();
				cBuilder.Widget(Widgets::cComboBox).Title("Test combo").Font(F::WhitneyMenu)
					.Entries({ "Case 1", "Case 2", "Case 3", "Case 1", "Case 2", "Case 3", "Case 1", "Case 2", "Case 3", "Case 1", "Case 2", "Case 3", "Case 1", "Case 2", "Case 3" }).SpawnIn(Widgets::cGroupbox);

				Widgets::cTextBox = std::make_shared<FGUI::CTextBox>();
				cBuilder.Widget(Widgets::cTextBox).Title("Text tesrt").Position(10, 130).Font(F::WhitneyMenu).SpawnIn(Widgets::cGroupbox);

				Widgets::cCheckTest1 = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cCheckTest1).Title("Test Check 1").Position(10, 160).Font(F::WhitneyMenu).Tooltip("test tooltip").SpawnIn(Widgets::cGroupbox);

				Widgets::cCheckTest2 = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cCheckTest2).Title("Test Check 2").Font(F::WhitneyMenu).Tooltip("test tooltip").SpawnIn(Widgets::cGroupbox);

				Widgets::cCheckTest3 = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cCheckTest3).Title("Test Check 3").Font(F::WhitneyMenu).Tooltip("test tooltip").SpawnIn(Widgets::cGroupbox);

				Widgets::cSlider1 = std::make_shared<FGUI::CSlider>();
				cBuilder.Widget(Widgets::cSlider1).Title("Slider wesh 1").Font(F::WhitneyMenu).Prefix("ms").Range(0, 100).Value(0).SpawnIn(Widgets::cGroupbox);
			}

			Widgets::cGroupboxVisual = std::make_shared<FGUI::CContainer>();
			cBuilder.Widget(Widgets::cGroupboxVisual).Title("Esp").Position(10, 20).Size(240, 240).Font(F::WhitneyMenu).Medium(Widgets::cMainTabPanel, 1).SpawnIn(Widgets::cMainMenu, false);
			{
				Widgets::cItemSelector = std::make_shared<FGUI::CItemSelector>();
				cBuilder.Widget(Widgets::cItemSelector).Title("Item test").Position(10, 20).Font(F::WhitneyMenu)
					.Entries({"test 1", "test 2", "iteem 3", "item 5"})
					.SpawnIn(Widgets::cGroupboxVisual);
			}

			Widgets::cGroupboxSkins = std::make_shared<FGUI::CContainer>();
			cBuilder.Widget(Widgets::cGroupboxSkins).Title("Skin Changer").Position(10, 20).Size(240, 240).Font(F::WhitneyMenu).Medium(Widgets::cMainTabPanel, 2).SpawnIn(Widgets::cMainMenu, false);
			{
				Widgets::cActiveSkinchanger = std::make_shared<FGUI::CCheckBox>();
				cBuilder.Widget(Widgets::cActiveSkinchanger).Title("Enable").Position(10, 20).Font(F::WhitneyMenu).SpawnIn(Widgets::cGroupboxSkins);

				Widgets::cKnifeModel = std::make_shared<FGUI::CComboBox>();
				cBuilder.Widget(Widgets::cKnifeModel).Title("Knife model").Position(10, 60).Font(F::WhitneyMenu).SpawnIn(Widgets::cGroupboxSkins);

				Widgets::cWeaponSelection = std::make_shared<FGUI::CComboBox>();
				cBuilder.Widget(Widgets::cWeaponSelection).Title("Weapons").Position(10, 100).Font(F::WhitneyMenu).Callback(UpdateSkinList).SpawnIn(Widgets::cGroupboxSkins);

				Widgets::cSkinSelection = std::make_shared<FGUI::CComboBox>();
				cBuilder.Widget(Widgets::cSkinSelection).Title("Skins").Position(10, 140).Font(F::WhitneyMenu).SpawnIn(Widgets::cGroupboxSkins).Entry("Default").Callback(AddSkinForAWeapon);

				Widgets::cFullUpdate = std::make_shared<FGUI::CButton>();
				cBuilder.Widget(Widgets::cFullUpdate).Title("UPDATE").Position(10, 180).Font(F::WhitneyMenu).Callback(U::ForceFullUpdate).SpawnIn(Widgets::cGroupboxSkins);
			}

			Widgets::cGroupboxConfig = std::make_shared<FGUI::CContainer>();
			cBuilder.Widget(Widgets::cGroupboxConfig).Title("Config").Position(10, 10).Size(240, 280).Font(F::WhitneyMenu).Medium(Widgets::cMainTabPanel, 4).ScrollBar(true).SpawnIn(Widgets::cMainMenu, false);
			{
				Widgets::cConfigList = std::make_shared<FGUI::CListBox>();
				cBuilder.Widget(Widgets::cConfigList).Title("Configs").Font(F::WhitneyMenu).Size(250, 100).SpawnIn(Widgets::cGroupboxConfig);

				Widgets::cConfigName = std::make_shared<FGUI::CTextBox>();
				cBuilder.Widget(Widgets::cConfigName).Title("Config name").Font(F::WhitneyMenu).SpawnIn(Widgets::cGroupboxConfig);

				Widgets::cSaveButton = std::make_shared<FGUI::CButton>();
				cBuilder.Widget(Widgets::cSaveButton).Title("Save").Font(F::WhitneyMenu).Callback(W::Save).SpawnIn(Widgets::cGroupboxConfig);

				Widgets::cLoadButton = std::make_shared<FGUI::CButton>();
				cBuilder.Widget(Widgets::cLoadButton).Title("Load").Font(F::WhitneyMenu).Callback(W::Load).SpawnIn(Widgets::cGroupboxConfig);

				Widgets::cCreateButton = std::make_shared<FGUI::CButton>();
				cBuilder.Widget(Widgets::cCreateButton).Title("Create").Font(F::WhitneyMenu).Callback(W::Save).SpawnIn(Widgets::cGroupboxConfig);

				Widgets::cRemoveButton = std::make_shared<FGUI::CButton>();
				cBuilder.Widget(Widgets::cRemoveButton).Title("Remove").Font(F::WhitneyMenu).Callback(W::Load).SpawnIn(Widgets::cGroupboxConfig);
			}
		}
	}

	RefreshConfigList();
	InitWeaponList();
}

void W::AddSkinForAWeapon()
{
	EItemDefinitionIndex weaponID = static_cast<EItemDefinitionIndex>(Widgets::cWeaponSelection->GetValue());
	auto weaponSkins = &CSkinChanger::Get().weaponSkins;
	if (weaponSkins->find(weaponID) == weaponSkins->end())
	{
		weaponSkins->emplace(weaponID, Widgets::cSkinSelection->GetValue());
	}
	else
	{
		weaponSkins->at(weaponID) = Widgets::cSkinSelection->GetValue();
	}
}

void W::UpdateAccentColor()
{
	W::Widgets::cMainMenu->SetAccentColor(W::Widgets::cColorPicker->GetColor());
}

void W::InitWeaponList()
{
	for (const auto& skin : mapItemListWeapon)
	{
		Widgets::cWeaponSelection->AddEntry(skin.second.szName, skin.first);
	}

	Widgets::cKnifeModel->AddEntry("default", 0);
	for (const auto& skin : mapItemListKnife)
	{
		Widgets::cWeaponSelection->AddEntry(skin.second.szName, skin.first);
		Widgets::cKnifeModel->AddEntry(skin.second.szName, skin.first);
	}
}

void W::UpdateSkinList()
{
	Widgets::cSkinSelection->ClearEntries();
	for (const auto& skin : CSkinChanger::Get().GetSkinsForAWeapon(static_cast<EItemDefinitionIndex>(Widgets::cWeaponSelection->GetValue())))
	{
		Widgets::cSkinSelection->AddEntry(skin.name, skin.iPaintKit);
	}
}

void W::Save()
{
	Widgets::cMainMenu->SaveToFile("dominoware\\" + Widgets::cConfigList->GetSelectedName() + ".dw");
	RefreshConfigList();
}

void W::Load()
{
	Widgets::cMainMenu->LoadFromFile("dominoware\\" + Widgets::cConfigList->GetSelectedName() + ".dw");
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

void W::RefreshConfigList()
{
	Widgets::cConfigList->ClearEntries();
	for (const auto& pFile : std::filesystem::directory_iterator("dominoware\\"))
	{
		auto file_path = pFile.path().string();
		auto file_name = split(file_path, "\\")[1];
		auto display_file_name = split(file_name, ".")[0];
		Widgets::cConfigList->AddEntry(display_file_name);
	}
}

void W::Render()
{
	Widgets::cMainMenu->Render();
}