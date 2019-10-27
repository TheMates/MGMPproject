#pragma once
#include <opencv2/core/core.hpp>

/// <summary> Class/interface IMovable. Handles moving in 3D scene. \n
/// Contains position, view direction, moving direction, view rotation weights, azimuth, elevation. 
/// Different behaviour for camera and not camera objects. </summary>
class IMovable
{
protected:
	cv::Point3f position = { 0.0, 0.0, 0.0 };
	cv::Vec3f viewDirection = { 0.0f, 0.0f, -1.0f };
	cv::Vec3f movingDirection = { 0.0f, 0.0f, -1.0f };
	cv::Vec3f viewRotationWeights = { 1.0f ,-1.0f ,0.0f };
	float azimuth = 0.f;
	float elevation = 0.0f;
	bool isCamera = false;
	bool canMoveVertically = true;

public:
	virtual ~IMovable() = default;
	//GET
	cv::Point3f virtual GetPosition() const;
	cv::Vec3f virtual GetViewDirection() const;
	cv::Vec3f virtual GetMovingDirection() const;
	cv::Vec3f virtual GetViewRotationWeights() const;
	bool GetVerticalMovementFlag();

	//SET
	void virtual SetPosition(float xPosition, float yPosition, float zPosition);
	void virtual SetPosition(cv::Point3f newPosition);
	void virtual SetViewDirection(float xAngle, float yAngle, float zAngle);
	void virtual SetViewDirection(cv::Vec3f newRotation);
	void virtual SetMovingDirection(float xAngle, float yAngle, float zAngle);
	void virtual SetMovingDirection(cv::Vec3f newRotation);
	void SetVerticalMovement(bool shouldMoveVertically);

	//MOVE
	void virtual MoveForward(float step);
	void virtual MoveBackwards(float step);
	void virtual MoveLeft(float step);
	void virtual MoveRight(float step);

	void virtual Rotate(float xAngleDelta, float yAngleDelta, float zAngleDelta);
	void virtual Rotate(cv::Vec3f newRotationAngleDelta);
	void virtual RotateAngle(float xAngle, float yAngle, float zAngle);

	void virtual ResetPosition();

	//PERFORM
	void virtual PerformTransformation();

};
