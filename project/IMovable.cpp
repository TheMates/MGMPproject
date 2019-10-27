#include "IMovable.h"
#include "MGMPprojectNamespace.h"
#include <GL/glut.h>

//GET
cv::Point3f IMovable::GetPosition() const
{
	if (isCamera)
		return -position;

	return position;
}
cv::Vec3f IMovable::GetViewDirection() const
{
	return viewDirection;
}
cv::Vec3f IMovable::GetMovingDirection() const
{
	return movingDirection;
}
cv::Vec3f IMovable::GetViewRotationWeights() const
{
	return viewRotationWeights;
}
bool IMovable::GetVerticalMovementFlag()
{
	return canMoveVertically;
}

//SET
void IMovable::SetVerticalMovement(bool shouldMoveVertically)
{
	canMoveVertically = shouldMoveVertically;
}
void IMovable::SetPosition(float xPosition, float yPosition, float zPosition)
{
	position.x = -xPosition;
	position.y = -yPosition;
	position.z = -zPosition;
}
void IMovable::SetPosition(cv::Point3f newPosition)
{
	SetPosition(newPosition.x, newPosition.y, newPosition.z);
}
void IMovable::SetViewDirection(float xAngle, float yAngle, float zAngle)
{
	viewDirection[0] = xAngle;
	viewDirection[1] = yAngle;
	viewDirection[2] = zAngle;
}
void IMovable::SetViewDirection(cv::Vec3f newRotation)
{
	SetViewDirection(newRotation[0], newRotation[1], newRotation[2]);
}
void IMovable::SetMovingDirection(float xAngle, float yAngle, float zAngle)
{
	movingDirection[0] = xAngle;
	movingDirection[1] = yAngle;
	movingDirection[2] = zAngle;
}
void IMovable::SetMovingDirection(cv::Vec3f newRotation)
{
	SetMovingDirection(newRotation[0], newRotation[1], newRotation[2]);
}

//MOVE
void IMovable::MoveForward(float step)
{
	if (isCamera)
	{
		position.x -= step * movingDirection[0];
		position.y -= step * movingDirection[1];
		position.z -= step * movingDirection[2];
	}
	else
	{
		position.x += step * movingDirection[0];
		position.y += step * movingDirection[1];
		position.z += step * movingDirection[2];
	}
}
void IMovable::MoveBackwards(float step)
{
	if (isCamera)
	{
		position.x += step * movingDirection[0];
		position.y += step * movingDirection[1];
		position.z += step * movingDirection[2];
	}
	else
	{
		position.x -= step * movingDirection[0];
		position.y -= step * movingDirection[1];
		position.z -= step * movingDirection[2];
	}
}
void IMovable::MoveLeft(float step)
{
	if (isCamera)
	{
		position.x += step * cosf(PI* azimuth / 180.0f);
		position.z -= step * sinf(PI* azimuth / 180.0f);
	}
	else
	{
		position.x -= step * cosf(PI* azimuth / 180.0f);
		position.z += step * sinf(PI* azimuth / 180.0f);
	}
}
void IMovable::MoveRight(float step)
{
	if (isCamera)
	{
		position.x -= step * cosf(PI* azimuth / 180.0f);
		position.z += step * sinf(PI* azimuth / 180.0f);
	}
	else
	{
		position.x += step * cosf(PI* azimuth / 180.0f);
		position.z -= step * sinf(PI* azimuth / 180.0f);
	}
}

void IMovable::Rotate(float xAngleDelta, float yAngleDelta, float zAngleDelta)
{
	// expecting deltaX - horizontal mouse movement, deltaY - vertical mouse movement
	// deltaX - change of azimuth, deltaY - change of elevation
	//then recalculate viewDirectionVector from these 2 angles

	if (isCamera)
	{
		azimuth -= xAngleDelta * PIover180;
		elevation -= yAngleDelta * PIover180;
	}
	else
	{
		azimuth += xAngleDelta * PIover180;
		elevation += yAngleDelta * PIover180;
	}
	//then do the checking if it overflew or not
	if (azimuth > 180.0f)
		azimuth = -(360.0f - azimuth);
	else if (azimuth < -180.f)
		azimuth = (360 + azimuth);
	if (elevation > 90.0f)
		elevation = 90.0f;
	if (elevation < -90.0f)
		elevation = -90.0f;

	//https://math.stackexchange.com/questions/1150232/finding-the-unit-direction-vector-given-azimuth-and-elevation

	//TODO: with flag vertical movement calculate these
	if(isCamera)
	{
		viewDirection[0] = -sinf(PIover180* azimuth)*cosf(PIover180* elevation);		//x
		viewDirection[2] = -cosf(PIover180* azimuth)*cosf(PIover180* elevation);		//y
		viewDirection[1] = -sinf(PIover180* elevation);		//z

		if (canMoveVertically)
		{
			movingDirection[0] = viewDirection[0];
			movingDirection[2] = viewDirection[2];
			movingDirection[1] = viewDirection[1];
		}
		else
		{
			movingDirection[0] = -sinf(PI* azimuth / 180.0f);		//x
			movingDirection[2] = -cosf(PI* azimuth / 180.0f);		//y
			movingDirection[1] = 0.0;		//z
		}
	}
	else
	{
		viewDirection[0] = sinf(PIover180* azimuth )*cosf(PIover180* elevation );		//x
		viewDirection[2] = cosf(PIover180* azimuth )*cosf(PIover180* elevation );		//y
		viewDirection[1] = sinf(PIover180* elevation );		//z
		if (canMoveVertically)
		{
			movingDirection[0] = viewDirection[0];
			movingDirection[2] = viewDirection[2];
			movingDirection[1] = viewDirection[1];
		}
		else
		{
			movingDirection[0] = sinf(PIover180* azimuth );		//x
			movingDirection[2] = cosf(PIover180* azimuth );		//y
			movingDirection[1] = 0.0;		//z
		}
	}

	if(isCamera)
	{
		viewRotationWeights[1] = -1.0;	//for azimuth, y rotation
		viewRotationWeights[0] = cosf(PIover180* azimuth );	//x rotation
		viewRotationWeights[2] = -sinf(PIover180* azimuth );	//z rotation
	}
	else
	{
		viewRotationWeights[1] = 1.0;	//for azimuth, y rotation
		viewRotationWeights[0] = -cosf(PIover180* azimuth );	//x rotation
		viewRotationWeights[2] = sinf(PIover180* azimuth );	//z rotation
	}
}
void IMovable::Rotate(cv::Vec3f newRotationAngleDelta)
{
	Rotate(newRotationAngleDelta[0], newRotationAngleDelta[1], newRotationAngleDelta[2]);
}
void IMovable::RotateAngle(float xAngle, float yAngle, float zAngle)
{
	Rotate(xAngle/PIover180, yAngle/PIover180, zAngle/PIover180);
}

void IMovable::ResetPosition()
{
	position = { 0.0, 0.0, 0.0 };
	viewDirection = { 0.0f, 0.0f, -1.0f };
	movingDirection = { 0.0f, 0.0f, -1.0f };
	viewRotationWeights = { 0.0, 1.0, 1.0 };
	azimuth = elevation = 0.0f;
}

//PERFORM
void IMovable::PerformTransformation()
{
	glRotatef(azimuth, 0.0f, viewRotationWeights[1], 0.0f);						// x axis rotation
	glRotatef(elevation, viewRotationWeights[0], 0.0f, viewRotationWeights[2]); // x and z axis rotation
	glTranslatef(position.x, position.y, position.z);
}

