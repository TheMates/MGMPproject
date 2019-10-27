#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include "World.h"
#include "Player.h"

/// <summary> Game class, handles menus, displaying lives and levels. </summary>
class Game
{
	World *world;
	Player *player;

public:
	Game() = default;
	void createMenu(void(*func)(int value));
	void onMenu(int value);
	void setWorld(World &world);
	void setPlayer(Player &player);
	void DrawStats() const;

};

#endif