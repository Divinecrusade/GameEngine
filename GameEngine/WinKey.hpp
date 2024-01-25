#pragma once

#include <WinUser.h>


namespace GameEngine
{
	enum class WinKey	// not all keys presented
	{
		MOUSE_LEFT_BUTTON = VK_LBUTTON,
		MOUSE_RIGHT_BUTTON = VK_RBUTTON,
		MOUSE_MIDDLE_BUTTON = VK_MBUTTON,
		CTRL_BREAK = VK_CANCEL,
		CTRL = VK_CONTROL,
		CTRL_LEFT = VK_LCONTROL,
		CTRL_RIGHT = VK_RCONTROL,
		BACKSPACE = VK_BACK,
		TAB = VK_TAB,
		ENTER = VK_RETURN,
		SHIFT = VK_SHIFT,
		SHIFT_LEFT = VK_LSHIFT,
		SHIFT_RIGHT = VK_RSHIFT,
		ALT = VK_MENU,
		ALT_LEFT = VK_LMENU,
		ALT_RIGHT = VK_RMENU,
		CAPS_LOCK = VK_CAPITAL,
		SPACEBAR = VK_SPACE,
		ARROW_LEFT = VK_LEFT,
		ARROW_RIGHT = VK_RIGHT,
		ARROW_DOWN = VK_DOWN,
		ARROW_UP = VK_UP,
		NOT_PRESSED = -1
	};
}