#pragma once
#include "IMovable.h"
#include "OBJ_Loader.h"

/// <summary> Car class, inherits from IMovable. Handles drawing models, collisions, animating. </summary>
class Car : public IMovable
{
	int id;
	float movingSpeed = 0.2f;
	float collisionFromCenterDistance = 1.5;
	float drawXlimit = 50.0f;
	float drawZlimit = 50.0f;

	bool isOutOfLimit = false;
	objl::Loader* modelLoader;

public:
	Car(int id, float xPosition, float yPosition, float zPosition, float speed, objl::Loader &modelLoader);
	Car(int id, cv::Point3f position, float speed, objl::Loader &modelLoader);
	int Id() const;
	void DrawCar() const;
	void Animate();
	bool IsOutOfLimit() const;
	cv::Point3f getFrontCollisionPoint();
	cv::Point3f getRearCollisionPoint();
};


