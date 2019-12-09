#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <thread>

#include "PluginHelper.h"

/*
	Reads wheel button input and sends them to rbr as keystrokes.
*/
class WheelInputController
{
public:

	/*
		Initializes DirectInput device. (aka Racing wheel)
	*/
	bool initialize();

	/*
		Starts a new thread which calls listenForInput().
	*/
	void start();

	/*
		Creates an infinite loop, which checks if button X is pressed and if so, sends keystroke to RBR.
	*/
	void listenForInput();

	/*
		Initializes wheel capabilities.
	*/
	bool initCapabilities();

	/*
		Obtains access to the wheel.
	*/
	bool acquireWheel();

	/*
		Lets go access of the wheel.
	*/
	bool unacquireWheel();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_device;

	DIJOYSTATE m_deviceState;
	DIJOYSTATE m_deviceOldState;

	std::thread m_thread;
	bool m_requiresPolling;
};

