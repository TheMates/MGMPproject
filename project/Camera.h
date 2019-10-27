#pragma once
#include "IMovable.h"

/// <summary> Camera class.\n Has position and viewDirection. Position initialized to 0.0, 0.0, 0.0. \n 
/// View direction initially set to 0.0, 0.0, -1.0. </summary>
class Camera : public IMovable 
{
	float fov = 70.f;
	float nearPlane = 0.1f, farPlane = 350.0f,
		cameraSpeed = 0.1f, cameraSensitivity = 6.0f;
	bool debug = false;
public:
	Camera();

	//GET
	float GetFov() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetCameraSpeed() const;
	float GetCamreaSensitivity() const;
	bool GetDebug() const;
	
	//SET
	void SetFov(float fovValue);
	void SetNearPlane(float nearPlaneValue);
	void SetFarPlane(float farPlaneValue);
	void SetCameraSpeed(float cameraSpeedValue);
	void SetCamreaSensitivity(float cameraSensitivityValue);
	void SetDebug(bool printDebugInfo);
	// PERFORMING METHODS 
	void PerformTransformation() override;
	void DrawDebug();
	void OnReshape() const;

	void KeyboardInput(unsigned char key);
};
