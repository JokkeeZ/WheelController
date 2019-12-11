#pragma once
#include <Windows.h>
#include <iostream>

/*
	Provides functionality to get current game state.
	Used for acquiring/unacquiring the wheel.
*/
class GameStateHandler
{
public:
	/*
		Enum indicating game state.
	*/
	enum class GameState
	{
		/*
			Player is driving.
		*/
		Driving,

		/*
			Everything else.
		*/
		Default
	};

	/*
		Gets new game state from memory. 
	*/
	GameState getNewGameState();

	/*
		Initializes this instance. Basically just opens process for memory reading.
	*/
	void initialize();

	/*
		Returns true, if initialization was success; otherwise false.
	*/
	bool isInitialized();

	/*
		Gets current game state.
	*/
	GameState getCurrentGameState();

	/*
		Sets current game state.
	*/
	void setCurrentGameState(GameState gameState);

private:
	HANDLE m_handle;
	bool m_initialized = false;
	GameState m_currentGameState = GameState::Default;
};

