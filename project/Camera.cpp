#include "Camera.h"
#include <Gl/glut.h>
#include "MGMPprojectNamespace.h"

#include <string>
#include <sstream>



using namespace MGMPproject;

Camera::Camera()
{
	isCamera = true;
}

// GET
float Camera::GetFov() const {return fov;}
float Camera::GetNearPlane() const {return nearPlane;}
float Camera::GetFarPlane() const {return farPlane;}
float Camera::GetCameraSpeed() const { return cameraSpeed; }
float Camera::GetCamreaSensitivity() const { return cameraSensitivity; }
bool Camera::GetDebug() const
{
	return debug;
}

//SET
void Camera::SetNearPlane(float nearPlaneValue) { nearPlane = nearPlaneValue; }
void Camera::SetFarPlane(float farPlaneValue) { farPlane = farPlaneValue; }
void Camera::SetCameraSpeed(float cameraSpeedValue)
{
	if(cameraSpeedValue>0.0001f)
		cameraSpeed = cameraSpeedValue;
}
void Camera::SetCamreaSensitivity(float cameraSensitivityValue)
{
	if(cameraSensitivityValue>0.f)
	cameraSensitivity = cameraSensitivityValue;
}
void Camera::SetFov(float fovValue)
{
	if (fovValue > 0.f && fovValue<180.f)
	{
		fov = fovValue;
		OnReshape();
	}
}
void Camera::SetDebug(bool printDebugInfo)
{
	debug = printDebugInfo;
}

//PERFORM
void Camera::PerformTransformation()
{
	glLoadIdentity();
	
	glRotatef(azimuth, 0.0f, viewRotationWeights[1], 0.0f); // x axis rotation
	glRotatef(elevation, viewRotationWeights[0], 0.0f, viewRotationWeights[2]); // x and z axis rotation
	glTranslatef(position.x, position.y, position.z); 
}
void Camera::DrawDebug()
{
	if (debug)
	{
		std::ostringstream infoStream;

		infoStream << "Position: x: " << -position.x <<
			" y: " << -position.y <<
			" z: " << -position.z;
		drawText(infoStream.str(), 10, 10);

		infoStream.str("");
		infoStream << "Azimuth: : " << -azimuth <<
			" Elevation: " << -elevation;
		drawText(infoStream.str(), 10, 20);
		infoStream.str("");
		infoStream << "View direction vector: x: " << viewDirection[0] <<
			" y: " << viewDirection[1] <<
			" z: " << viewDirection[2];
		drawText(infoStream.str(), 10, 30);
		
		infoStream.str("");
		infoStream << "FOV: " << fov;
		drawText(infoStream.str(), 10, 40);

		infoStream.clear();
	}
}
void Camera::OnReshape() const
{
	auto w = glutGet(GLUT_WINDOW_WIDTH);
	auto h = glutGet(GLUT_WINDOW_HEIGHT);
	glViewport(0, 0, w, h);             // OpenGL: nastaveni rozmenu viewportu

	glMatrixMode(GL_PROJECTION);        // OpenGL: matice bude typu projekce
	glLoadIdentity();                   // OpenGL: matice bude identicka (jen jednicky v hlavni diagonale)
	gluPerspective(fov, (double)w / (double)h, nearPlane, farPlane);
}

void Camera::KeyboardInput(unsigned char key)
{
	switch (key)
	{
	case 't': SetFov(fov + 5);  break;
	case 'g': SetFov(fov - 5);  break;
	case 'r': SetCamreaSensitivity(cameraSensitivity + 0.5f); break;
	case 'f': SetCamreaSensitivity(cameraSensitivity - 0.5f); break;
	case 'z': SetCameraSpeed(cameraSpeed + 0.1); break;
	case 'h': SetCameraSpeed(cameraSpeed - 0.1); break;
	default: break;
	}
}

