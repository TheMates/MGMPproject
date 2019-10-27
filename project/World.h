#pragma once
#ifndef _WORLD_H_
#define _WORLD_H_

#include <GL/glut.h>
#include <vector>
#include "Car.h"
#include "Player.h"
#include "OBJ_Loader.h"

//ANIMATIONS ARE CALLED ROUGHLY 60 FPS (17ms)
/// <summary> World class. Handles cars animations, drawing textures and models. Automatic car generation and destruction. Checks collisions. </summary>
class World
{
	Player* player;
	MGMPproject::WorldBorders worldBorders{ 70.f,-0.25f,-50.f,50.f,-130.f,10.f };

	//TEXTURES
	static const int nTextures = 4;
	GLuint textures[nTextures];
	std::vector<int> currentTexturesMask;	// 0) road  1) grass  2) sand  3) dirt
	int nGeneratedTextures = 7;
	const float textureWidth = 40.f;
	const float textureHeight = 20.f;
	void drawTexture(int texture);
	//MODELS
	//
	objl::Loader trabantLoader;
	objl::Loader crownLoader;

	//CARS
	std::vector<Car> cars = {};
	unsigned int carsIds = 0;
	const float initialCarSpeed = 0.1f;
	void AddCar(float xPosition, float yPosition, float zPosition, float speed, float deltaAzimuth = 0.0f );
	void GenerateCars();
	void DestroyCar(int id);
	std::vector<Car>::iterator findCarById(int id);
	std::vector<int> timeElapsedFromLastCarGeneration;		//double the size of nGeneratedTextures
	std::vector<float> currentCarSpeedMask;					//double the size of nGeneratedTextures
	void generateNewElapseTime(int index);
	const int minimumFramesElapsed = 160;
	constexpr static const float carCollisionDistance = 2.f;

	//CROWN
	cv::Point3f crownPosition{ 0,0,-120.f };
	constexpr static const float crownActivationDistance = 2.0f;
	float crownAngle = 0.0f;

	//ANIMATION
	bool shouldAnimate = true;
	void generateNewMasks();

	//LOGIC
	int lives = 3;
	int levelsCompleted = 0;


public:
	World(Player &player);

	GLuint * Textures(int index);
	GLuint * Textures();
	void LoadTextures();
	void LoadModels();

	void DrawTextures();
	void ClearBuffers();
	void LoadNewLevel();

	void Animate();
	void SetShouldAnimate(bool value);
	void KeyboardInput(unsigned char key);

	// DRAW
	void DrawCars();
	void DrawCrown();

	// COLLISIONS
	void CheckCollisions();

	//LOGIC
	int GetLives() const;
	int GetLevelsCompleted() const;
	int GetNumCars() const;


};
#endif	


