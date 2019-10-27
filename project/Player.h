#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Camera.h"
#include "PlayerAnimator.h"


/// <summary> Player class, inherits from IMovable. Handles drawing player model, animating and drawing debug information. </summary>
class Player :public IMovable
{
	Camera *camera;
	bool isInJumpAnimation = false;
	bool debug = false;

	float jumpingAngle = 45;

	PlayerAnimator animator;
	objl::Loader modelLoader;



public:
	Player(Camera &camera);
	//GET
	bool GetDebug() const;

	//SET
	void SetDebug(bool printDebugInfo);
	
	//ANIMATION
	bool IsInAnimation() const;
	void Animate();
	void JumpForward();

	//PERFORM
	void PerformTransformation() override;
	void drawPlayer();
	void DrawDebug();

	void CheckKeyboardInput();
	void KeyboardInput(unsigned char key);

	void ResetPosition() override;
	void LoadModels();


	
};

#endif