#ifndef _PLAYERANIMATOR_H_
#define _PLAYERANIMATOR_H_

#include "MGMPprojectNamespace.h"
#include <opencv2/core/core.hpp>

/// <summary> Player animator class handles the actual animation fo jump for player. </summary>
class PlayerAnimator
{
	
	float azimuth = 0.f;									//from player
	//float elevation = 0.0f;								//from player
	cv::Point3f position;
	cv::Point3f originalPosition;
	float jumpingAngle = 45;							//from player

	float v0 = 17.0;								
	float g = 9.81*6;										
	float lengthOfJumpAnimation;						//will be calculated
	
	bool isInJumpAnimation = false;						//identifier
	float currentJumpForwardAnimationTime = 0.0;

public:

	PlayerAnimator(): lengthOfJumpAnimation(0), isInJumpAnimation(false ){  }

	void setParameters(float azimuth, cv::Point3f position, float jumpingAngle)
	{
		this->azimuth = azimuth;
		this->position = position;
		originalPosition = position;
		this->jumpingAngle = jumpingAngle;

		lengthOfJumpAnimation = 2*v0 * sinf(PI  * this->jumpingAngle / 180.0f) / g;
	}

	bool IsInJumpAnimation() const
	{
		return isInJumpAnimation;
	}

	void setIsAnimating(bool flag)
	{
		isInJumpAnimation = flag;
	}

	void recalculate()
	{
		currentJumpForwardAnimationTime += 0.016;
		const float newX = v0 * currentJumpForwardAnimationTime*cosf(PI*jumpingAngle / 180.0);
		const float newY = v0 * currentJumpForwardAnimationTime*cosf(PI*jumpingAngle / 180.0) - 0.5*g*currentJumpForwardAnimationTime*currentJumpForwardAnimationTime;

		float deltaY = -newY;
		float deltaZ = cosf(PI*azimuth / 180.0)*newX;
		float deltaX = sinf(PI*azimuth / 180.0)*newX;


		position.x = originalPosition.x + deltaX;
		position.y = originalPosition.y + deltaY;
		position.z = originalPosition.z + deltaZ;		
		if (currentJumpForwardAnimationTime > lengthOfJumpAnimation)
		{
			position.y = originalPosition.y;
			isInJumpAnimation = false;
			currentJumpForwardAnimationTime = 0.0;
		}
	}

	cv::Point3f GetPosition() const
	{
		return position;
	}
	
};

#endif
