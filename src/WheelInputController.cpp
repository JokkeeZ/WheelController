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

				// Player stopped driving OR
				// Player is in some menu OR
				// Something where wheel buttons are not in driving use.
				else {
					std::cout << "Wheel acquired!" << std::endl;
					acquireWheel();
				}

				// Lets save game state so we don't spam acquire/unacquire wheel.
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

		if (m_deviceState.rgbButtons[2] != NULL && m_deviceOldState.rgbButtons[2] != m_deviceState.rgbButtons[2]) {
			// ESC ('△' on wheel)
			sendKey(27, false);
		}

		// Some drivers report a value of 65,535, instead of -1, for the center position.
		bool POVCentered = (LOWORD(m_deviceState.rgdwPOV[0]) == 0xFFFF);
		int centerValue = POVCentered ? 0xFFFF : -1;

		if (m_deviceState.rgdwPOV[0] == 0 && m_deviceOldState.rgdwPOV[0] == centerValue) {
			// Up (Arrow up on wheel)
			sendKey(38, false);
		}

		if (m_deviceState.rgdwPOV[0] == 9000 && m_deviceOldState.rgdwPOV[0] == centerValue) {
			// Right (Arrow right on wheel)
			sendKey(39, false);
		}

		if (m_deviceState.rgdwPOV[0] == 18000 && m_deviceOldState.rgdwPOV[0] == centerValue) {
			// Down (Arrow down on wheel)
			sendKey(40, false);
		}

		if (m_deviceState.rgdwPOV[0] == 27000 && m_deviceOldState.rgdwPOV[0] == centerValue) {
			// Left (Arrow left on wheel)
			sendKey(37, false);
		}

		m_deviceOldState = m_deviceState;
		Sleep(25);
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
