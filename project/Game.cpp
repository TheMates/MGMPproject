#include <GL/glut.h>
#include "Game.h"
#include "MGMPprojectNamespace.h"


void Game::createMenu(void(* func)(int value))
{
	const int idAnimation = glutCreateMenu(func);
	glutAddMenuEntry("Run", RUN_ANIMATION);
	glutAddMenuEntry("Stop", STOP_ANIAMTION);

	glutCreateMenu(func);
	glutPostRedisplay();
	glutAddSubMenu("Animation", idAnimation);
	glutAddMenuEntry("Reset player position ", RESET_PLAYER_POSITION); 
	glutAddMenuEntry("Load new level ", WORLD_LOAD_LEVEL);
	glutAddMenuEntry("Quit", GAME_EXIT);
	glutPostRedisplay();

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void Game::onMenu(int value)
{
	switch (value)
	{
	case GAME_EXIT:  exit(1); break;
	case RUN_ANIMATION: world->SetShouldAnimate(true); ; break;
	case STOP_ANIAMTION: world->SetShouldAnimate(false); break;
	case RESET_PLAYER_POSITION: player->ResetPosition(); break;
	case WORLD_LOAD_LEVEL: world->LoadNewLevel(); break;
	default:break;
	}

}

void Game::setWorld(World& world)
{
	this->world = &world;
}

void Game::setPlayer(Player& player)
{
	this->player = &player;
}

void Game::DrawStats() const
{
	std::ostringstream infoStream;

	infoStream << "Lives: " << world->GetLives() <<
		" Levels completed: " << world->GetLevelsCompleted(); 

	MGMPproject::drawText(infoStream.str(), glutGet(GLUT_WINDOW_WIDTH)/2 - 100, 15,MGMPproject::FontSize::Large);
	if (player->GetDebug())
	{
		infoStream.str("");
		infoStream << "Cars: " << world->GetNumCars();
		MGMPproject::drawText(infoStream.str(), 10, 60);
	}
}

