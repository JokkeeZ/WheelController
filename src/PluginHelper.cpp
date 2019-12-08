#include "PluginHelper.h"

void sendKey(int vk, bool extended)
{
	KEYBDINPUT  kb = { 0 };
	INPUT       Input = { 0 };

	/* Generate a "key down" */
	if (extended) {
		kb.dwFlags = KEYEVENTF_EXTENDEDKEY;
	}

	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;

	SendInput(1, &Input, sizeof(Input));

	/* Generate a "key up" */
	ZeroMemory(&kb, sizeof(KEYBDINPUT));

	ZeroMemory(&Input, sizeof(INPUT));

	kb.dwFlags = KEYEVENTF_KEYUP;

	if (extended) {
		kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
	}

	kb.wVk = vk;
	Input.type = INPUT_KEYBOARD;
	Input.ki = kb;
	SendInput(1, &Input, sizeof(Input));
};