#pragma once
#include "../../dependencies/FGUI/FGUI.hpp"
// used: std::array
#include <array>
// used: winapi includes
#include "../common.h"
// used: logging
#include "logging.h"

/*
 * INPUT SYSTEM
 * listen and handle key states
 */
namespace IPT
{
	inline std::array<bool, 256> arrCurrentPressedKey, arrOldPressedKey;
	inline FGUI::POINT ptCursorPosition, ptCursorPositionDelta;

	void PullInput();
	bool IsKeyHeld(unsigned uKeyCode);
	bool IsKeyReleased(unsigned uKeyCode);
	bool IsKeyPressed(unsigned uKeyCode);

	bool GetMouseScrollWheel(bool down);
	inline bool bMousewheeldown = false;
	inline bool bMousewheelup = false;

	FGUI::POINT GetCursorPosition();
	FGUI::POINT GetCursorPositionDelta();

	bool IsCursorInArea(FGUI::AREA arArea);

	// Get
	/* set our keybind */
	void Setup();
}
