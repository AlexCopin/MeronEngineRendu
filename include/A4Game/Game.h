#pragma once
#include <SDL.h>
#include <iostream>

class GameInstance
{
public:
	GameInstance();
	GameInstance(const GameInstance&) = delete;
	GameInstance(GameInstance&&) = delete;
	~GameInstance();
	GameInstance& operator=(const GameInstance&) = delete;
	GameInstance& operator=(GameInstance&&) = delete;

	void SetPaused() { isPaused = isPaused ? false : true; }
	bool IsPaused() { return isPaused; }

	static GameInstance& Instance(); 


	void StartGame();
private:
	std::string m_gameName;
	static GameInstance* s_instance;
	bool isPaused;
};