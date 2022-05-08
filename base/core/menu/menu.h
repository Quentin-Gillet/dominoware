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
	};

	void Initialize();
	void Render();

	void UpdateAccentColor();
}
