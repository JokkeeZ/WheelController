#include "GameStateHandler.h"
#include "PluginHelper.h"

GameStateHandler::GameState GameStateHandler::getNewGameState()
{
	int buffer;
								// Base address
	ReadProcessMemory(m_handle, (void*)0x007EAC48, &buffer, sizeof(buffer), 0);
								// Offset from base address
	ReadProcessMemory(m_handle, (void*)(buffer + (0x738 - 0x10)), &buffer, sizeof(buffer), 0);

	if (buffer == 0x01) {
		return GameStateHandler::GameState::Driving;
	}

	return GameStateHandler::GameState::Default;
};

bool GameStateHandler::isInitialized()
{
	return m_initialized;
}

GameStateHandler::GameState GameStateHandler::getCurrentGameState()
{
	return m_currentGameState;
};

void GameStateHandler::setCurrentGameState(GameState gameState)
{
	m_currentGameState = gameState;
};

void GameStateHandler::initialize()
{
	// Open RBR process for reading.
	m_handle = OpenProcess(PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	if (!m_handle) {
		return;
	}

	// Save initialization state.
	m_initialized = true;
};
