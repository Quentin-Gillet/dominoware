// used: std::this_thread
#include <thread>

#include "inputsystem.h"
// used: wndproc hook, inputsystem interface
#include "../core/hooks.h"
// used: menu open state
#include "../core/menu/menu.h"

void IPT::Setup()
{
	FGUI::INPUT.PullInput = IPT::PullInput;
	FGUI::INPUT.IsKeyHeld = IPT::IsKeyHeld;
	FGUI::INPUT.IsKeyReleased = IPT::IsKeyReleased;
	FGUI::INPUT.IsKeyPressed = IPT::IsKeyPressed;
	FGUI::INPUT.GetCursorPos = IPT::GetCursorPosition;
	FGUI::INPUT.GetCursorPosDelta = IPT::GetCursorPositionDelta;
	FGUI::INPUT.IsCursorInArea = IPT::IsCursorInArea;
	FGUI::INPUT.GetMouseScrollWheel = IPT::GetMouseScrollWheel;

	FGUI::INPUT.SetInputType(FGUI::INPUT_TYPE::INPUT_SYSTEM);

	L::Print("[INPUT] Input initialized.");
}

void IPT::PullInput()
{
	static FGUI::POINT ptLastKnowPosition = { 0, 0 };

	std::copy(arrCurrentPressedKey.begin(), arrCurrentPressedKey.end(), arrOldPressedKey.begin());

	for (int i = 0; i < 115; i++)
	{
		S::EButtonCode keyCode = static_cast<S::EButtonCode>(i);
		arrCurrentPressedKey.at(i) = I::InputSystem->IsButtonDown(keyCode);
	}

	GetCursorPos(reinterpret_cast<LPPOINT>(&ptCursorPosition));

	ptCursorPositionDelta = { (ptCursorPosition.m_iX - ptLastKnowPosition.m_iX), (ptCursorPosition.m_iY - ptLastKnowPosition.m_iY) };

	ptLastKnowPosition = ptCursorPosition;

	// Mouse wheel
	static int iOldAnalogValue = 0;
	int iCurrAnalogWheelValue = I::InputSystem->GetAnalogValue(MOUSE_WHEEL);

	int iDelta = iCurrAnalogWheelValue - iOldAnalogValue;

	bMousewheeldown = (iDelta == -1);
	bMousewheelup = (iDelta == 1);

	iOldAnalogValue = iCurrAnalogWheelValue;
}

bool IPT::IsKeyHeld(unsigned uKeyCode)
{
	return arrCurrentPressedKey.at(uKeyCode);
}

bool IPT::IsKeyReleased(unsigned uKeyCode)
{
	return (!arrCurrentPressedKey.at(uKeyCode) && arrOldPressedKey.at(uKeyCode));
}

bool IPT::IsKeyPressed(unsigned uKeyCode)
{
	return (arrCurrentPressedKey.at(uKeyCode) && !arrOldPressedKey.at(uKeyCode));
}

FGUI::POINT IPT::GetCursorPosition()
{
	return ptCursorPosition;
}

FGUI::POINT IPT::GetCursorPositionDelta()
{
	return ptCursorPositionDelta;
}

bool IPT::IsCursorInArea(FGUI::AREA arArea)
{
	if (&arArea == nullptr)
		return false;
	if (arArea.m_iBottom == 0 && arArea.m_iLeft == 0 && arArea.m_iRight == 0 && arArea.m_iTop == 0)
		return false;
	return (GetCursorPosition().m_iX > arArea.m_iLeft && GetCursorPosition().m_iY > arArea.m_iTop &&
		GetCursorPosition().m_iX < arArea.m_iLeft + arArea.m_iRight && GetCursorPosition().m_iY < arArea.m_iTop + arArea.m_iBottom);
}

bool IPT::GetMouseScrollWheel(bool down)
{
	if (bMousewheeldown && down)
		return true;
	else if (bMousewheelup && !down)
		return true;
	else
		return false;
}