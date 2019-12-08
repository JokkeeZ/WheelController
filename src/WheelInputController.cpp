#include "WheelInputController.h"


bool WheelInputController::initialize()
{
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8A,
		(void**)&this->m_directInput,
		NULL
	);

	if (result != DI_OK) {
		return false;
	}

	if (FAILED(this->m_directInput->CreateDevice(GUID_Joystick, &this->m_device, NULL))) {
		return false;
	}

	this->m_device->SetDataFormat(&c_dfDIJoystick);
	this->m_device->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	return true;
};

void WheelInputController::start()
{
	m_thread = std::thread([this] { listenForInput(); });
}

void WheelInputController::listenForInput()
{
	while (true)
	{
		m_device->GetDeviceState(sizeof(DIJOYSTATE), &m_deviceState);

		if (m_deviceState.rgbButtons[5] != NULL) {
			// Enter ('X' on wheel)
			sendKey(13, false);
		}

		if (m_deviceState.rgbButtons[4] != NULL) {
			// ESC ('O' on wheel)
			sendKey(27, false);
		}

		if (m_deviceState.rgdwPOV[0] == 0) {
			// Up (Arrow up on wheel)
			sendKey(38, false);
		}

		if (m_deviceState.rgdwPOV[0] == 9000) {
			// Right (Arrow right on wheel)
			sendKey(39, false);
		}

		if (m_deviceState.rgdwPOV[0] == 18000) {
			// Down (Arrow down on wheel)
			sendKey(40, false);
		}

		if (m_deviceState.rgdwPOV[0] == 27000) {
			// Left (Arrow left on wheel)
			sendKey(37, false);
		}

		Sleep(150);
	}
}

bool WheelInputController::acquireWheel()
{
	// TODO: Handle errors better:
	// DIERR_INVALIDPARAM
	// DIERR_NOTINITIALIZED
	// DIERR_OTHERAPPHASPRIO
	HRESULT result = m_device->Acquire();
	return result == DI_OK;
};

bool WheelInputController::unacquireWheel()
{
	HRESULT result = m_device->Unacquire();
	
	// Device was not acquired, operation had no effect.
	if (result == DI_NOEFFECT) {
		return false;
	}

	return result == DI_OK;
};
