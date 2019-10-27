#include <GL/glut.h>
#include "Car.h"
#include "MGMPprojectNamespace.h"


Car::Car(int id, float xPosition, float yPosition, float zPosition, float speed, objl::Loader& modelLoader) :id(id), modelLoader(&modelLoader),movingSpeed(speed)
{
	position.x = xPosition;
	position.y = yPosition;
	position.z = zPosition;
}
Car::Car(int id, cv::Point3f position,float speed, objl::Loader& modelLoader): id(id), modelLoader(&modelLoader),movingSpeed(speed)
{
	this->position.x = position.x;
	this->position.y = position.y;
	this->position.z = position.z;
}

int Car::Id() const
{
	return id;
}
void Car::DrawCar() const
{
	glPushMatrix();
	glTranslatef(position.x, position.y-1, position.z);
	glScalef(1.5, 1.5, 1.5);
	glRotatef(-azimuth, 0, 1, 0);
	MGMPproject::drawBlenderObject(modelLoader, 0);

	glPopMatrix();

}
void Car::Animate()
{
	//with each call a car will move in its direction
	position.x += movingDirection[0] * movingSpeed;
	position.z += movingDirection[2] * movingSpeed;

	if (abs(position.x) > drawXlimit)
		isOutOfLimit = true;
}
bool Car::IsOutOfLimit() const
{
	return isOutOfLimit;
}

cv::Point3f Car::getFrontCollisionPoint()
{
	return cv::Point3f{position.x+movingDirection[0]* collisionFromCenterDistance,position.y,position.z+movingDirection[2] * collisionFromCenterDistance };
}
cv::Point3f Car::getRearCollisionPoint()
{
	return cv::Point3f{ position.x- movingDirection[0] * collisionFromCenterDistance,position.y,position.z- movingDirection[2] * collisionFromCenterDistance };
}
