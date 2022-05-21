#pragma once
// used: std::function
#include <functional>
// used: winapi, directx, imgui, fmt includes
#include "../../common.h"
// used: global variables
#include "../../global.h"
// used: configurations error logging
#include "../../utilities/logging.h"
// used: engine, inputsystem, convar interfaces
#include "../../core/interfaces.h"
// used: font, draw
#include "../../utilities/draw.h"
// used: skins list
#include "../../features/skinchanger.h"

#include "../../utilities.h"

/*
 * WINDOWS
 */
namespace W
{

	namespace Widgets
	{
		inline std::shared_ptr<FGUI::CContainer> cMainMenu;
		inline std::shared_ptr<FGUI::CTabPanel> cMainTabPanel;

		inline std::shared_ptr<FGUI::CContainer> cGroupbox;
		inline std::shared_ptr<FGUI::CCheckBox> cCheckTest;
		inline std::shared_ptr<FGUI::CColorPicker> cColorPicker;
		inline std::shared_ptr<FGUI::CSlider> cSlider;
		inline std::shared_ptr<FGUI::CSlider> cSlider1;
		inline std::shared_ptr<FGUI::CCheckBox> cCheckTest1;
		inline std::shared_ptr<FGUI::CCheckBox> cCheckTest2;
		inline std::shared_ptr<FGUI::CCheckBox> cCheckTest3;

		inline std::shared_ptr<FGUI::CTextBox> cTextBox;
		inline std::shared_ptr<FGUI::CComboBox> cComboBox;

		inline std::shared_ptr<FGUI::CContainer> cGroupboxVisual;
		inline std::shared_ptr<FGUI::CItemSelector> cItemSelector;

		inline std::shared_ptr<FGUI::CContainer> cGroupboxSkins;
		inline std::shared_ptr<FGUI::CCheckBox> cActiveSkinchanger;
		inline std::shared_ptr<FGUI::CComboBox> cKnifeModel;
		inline std::shared_ptr<FGUI::CComboBox> cWeaponSelection;
		inline std::shared_ptr<FGUI::CComboBox> cSkinSelection;
		inline std::shared_ptr<FGUI::CButton> cFullUpdate;

	};

	void Initialize();
	void Render();

	void InitWeaponList();

	void UpdateAccentColor();
	void UpdateSkinList();
	void AddSkinForAWeapon();

}
