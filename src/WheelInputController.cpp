#include "WheelInputController.h"

bool WheelInputController::initialize()
{
	HRESULT result = DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8A,
		(void**)&m_directInput,
		NULL
	);

	if (result != DI_OK) {
		return false;
	}

	if (FAILED(m_directInput->CreateDevice(GUID_Joystick, &m_device, NULL))) {
		return false;
	}

	if (!initCapabilities()) {
		return false;
	}

	m_device->SetDataFormat(&c_dfDIJoystick);
	m_device->SetCooperativeLevel(NULL, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	m_gameStateHandler = new GameStateHandler;
	m_gameStateHandler->initialize();

	return true;
};

bool WheelInputController::initCapabilities()
{
	DIDEVCAPS capabilities;
	capabilities.dwSize = sizeof(DIDEVCAPS);

	HRESULT result = m_device->GetCapabilities(&capabilities);
	if (result != DI_OK) {
		return false;
	}

	// Has flag that wheel requires polling.
	if ((capabilities.dwFlags & DIDC_POLLEDDATAFORMAT) != 0) {
		m_requiresPolling = true;
	}

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
		if (m_gameStateHandler->isInitialized()) {
			auto newGameState = m_gameStateHandler->getNewGameState();
			auto currentGameState = m_gameStateHandler->getCurrentGameState();

			if (currentGameState != newGameState) {
				// Player is loading a stage or driving, so we don't need to have wheel access.
				if (newGameState != GameStateHandler::GameState::Default) {
					std::cout << "Wheel unacquired!" << std::endl;
					unacquireWheel();
				}

				// Player is in some menu/pause screen OR 
				// somewhere where wheel buttons are not in driving use.
				else {
					std::cout << "Wheel acquired!" << std::endl;
					acquireWheel();
				}

				m_gameStateHandler->setCurrentGameState(newGameState);
				continue;
			}
		}

		// Poll, if device requires it to return data.
		if (m_requiresPolling) {
			m_device->Poll();
		}

		m_device->GetDeviceState(sizeof(DIJOYSTATE), &m_deviceState);

		if (m_deviceState.rgbButtons[5] != NULL && m_deviceOldState.rgbButtons[5] != m_deviceState.rgbButtons[5]) {
			// Enter ('X' on wheel)
			sendKey(13, false);
		}

		if (m_deviceState.rgbButtons[4] != NULL && m_deviceOldState.rgbButtons[4] != m_deviceState.rgbButtons[4]) {
			// ESC ('O' on wheel)
			sendKey(27, false);
		}

		// Some drivers report a value of 65,535, instead of -1, for the center position.
		int centerValue = (LOWORD(m_deviceState.rgdwPOV[0]) == 0xFFFF) ? 0xFFFF : -1;

		if (m_deviceState.rgdwPOV[0] != centerValue && m_deviceOldState.rgdwPOV[0] != m_deviceState.rgdwPOV[0]) {
			sendKey(m_mapArrowButtons[m_deviceState.rgdwPOV[0]], false);
		}

		m_deviceOldState = m_deviceState;
		Sleep(25);
	}
}

bool WheelInputController::acquireWheel()
{
	HRESULT result = m_device->Acquire();
	return result == DI_OK;
};

bool WheelInputController::unacquireWheel()
{
	HRESULT result = m_device->Unacquire();
	return result == DI_OK;
};
