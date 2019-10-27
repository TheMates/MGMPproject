#include "Player.h"
#include "MGMPprojectNamespace.h"
#include <GL/glut.h>
#include <Windows.h>

Player::Player(Camera& camera)
{
	this->camera = &camera;
	isCamera = true;
}

//GET
bool Player::GetDebug() const
{
	return debug;
}

//SET
void Player::SetDebug(bool printDebugInfo)
{
	debug = printDebugInfo;
}

//ANIMATION
bool Player::IsInAnimation() const
{
	return isInJumpAnimation;	//případně && další animace
}
void Player::Animate()
{
	if (isInJumpAnimation)
		JumpForward();
}
void Player::JumpForward()	
{
	if (!animator.IsInJumpAnimation())	
	{
		isInJumpAnimation  = true;
		animator.setIsAnimating(true);
		animator.setParameters(azimuth, position, jumpingAngle);
	}

	if (animator.IsInJumpAnimation())
		animator.recalculate();
	position = animator.GetPosition();

	if (!animator.IsInJumpAnimation())
		isInJumpAnimation  = false;

}

//PERFORM
void Player::PerformTransformation()
{
	glLoadIdentity();

	drawPlayer();
	//rotate my camera 
	glRotatef(azimuth, 0.0f, viewRotationWeights[1], 0.0f); // x axis rotation
	glRotatef(elevation, viewRotationWeights[0], 0.0f, viewRotationWeights[2]); // x and z axis rotation
	glTranslatef(position.x, position.y, position.z);

	//push it back a little, so i can see the player
	glTranslatef(5*viewDirection[0], 5*viewDirection[1]-1.5, 5*viewDirection[2]);

}
void Player::drawPlayer()
{
	glPushMatrix();
	//get player to desired location
	glRotatef(azimuth, 0.0f, viewRotationWeights[1], 0.0f);					
	glRotatef(elevation, viewRotationWeights[0], 0.0f, viewRotationWeights[2]); 
	glTranslatef(5 * viewDirection[0], 5 * viewDirection[1]-2.5, 5 * viewDirection[2]);

	glRotatef(-azimuth, 0.0f, viewRotationWeights[1], 0.0f);	//rotate back, so the player looks the same direction as camera

	//DRAW PLAYER
	MGMPproject::drawBlenderObject(&modelLoader, 0);
	MGMPproject::drawBlenderObject(&modelLoader, 1);

	glPopMatrix();

}
void Player::DrawDebug()
{
	if (debug)
	{
		std::ostringstream infoStream;

		infoStream << "Position: x: " << -position.x <<
			" y: " << -position.y <<
			" z: " << -position.z;
		MGMPproject::drawText(infoStream.str(), 10, 10);

		infoStream.str("");
		infoStream << "Azimuth: : " << -azimuth <<
			" Elevation: " << -elevation;
		MGMPproject::drawText(infoStream.str(), 10, 20);
		infoStream.str("");
		infoStream << "View direction vector: x: " << viewDirection[0] <<
			" y: " << viewDirection[1] <<
			" z: " << viewDirection[2];
		MGMPproject::drawText(infoStream.str(), 10, 30);
		infoStream.str("");
		infoStream << "Moving direction vector: x: " << movingDirection[0] <<
			" y: " << movingDirection[1] <<
			" z: " << movingDirection[2];
		MGMPproject::drawText(infoStream.str(), 10, 40);


		infoStream.str("");
		infoStream << "FOV: " << camera->GetFov();
		MGMPproject::drawText(infoStream.str(), 10, 50);

		infoStream.clear();
	}
}

void Player::CheckKeyboardInput()
{
	if (GetKeyState('W') & 0x8000/*Check if high-order bit is set (1 << 15)*/)
	{
		if (GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)	//move slower
			MoveForward(camera->GetCameraSpeed()*0.707);
		else
			MoveForward(camera->GetCameraSpeed());
	}
	if (GetKeyState('S') & 0x8000)
	{
		if (GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)	//move slower
			MoveBackwards(camera->GetCameraSpeed() *0.707);
		else
			MoveBackwards(camera->GetCameraSpeed());
	}
	if (GetKeyState('A') & 0x8000)
	{
		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000)	//move slower
			MoveLeft(camera->GetCameraSpeed() *0.707);
		else
			MoveLeft(camera->GetCameraSpeed());
	}
	if (GetKeyState('D') & 0x8000)
	{
		if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000)	//move slower
			MoveRight(camera->GetCameraSpeed() *0.707);
		else
			MoveRight(camera->GetCameraSpeed());
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (!IsInAnimation()) 
			JumpForward();
	}
}
void Player::KeyboardInput(unsigned char key)
{
	switch (key)
	{
	case 'O':
	case 'o':	ResetPosition();break;
	case 'V':
	case 'v': SetVerticalMovement(!canMoveVertically); break;

	default: break;
	}
}

void Player::ResetPosition()
{
	isInJumpAnimation = false;
	animator.setIsAnimating(false);
	position = { 0.0, 0.0, 0.0 };
	viewDirection = { 0.0f, 0.0f, -1.0f };
	movingDirection = { 0.0f, 0.0f, -1.0f };
	viewRotationWeights = { 0.0, 1.0, 1.0 };
	azimuth = elevation = 0.0f;
	
	animator.setParameters(0, position, jumpingAngle);
}
void Player::LoadModels()
{
	modelLoader.LoadFile("models/zaba.obj");
}



