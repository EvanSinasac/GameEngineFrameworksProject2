#include "cCmd_RotateToRelativeTime_1.h"
#include <iostream>



cCmd_RotateToRelativeTime_1::cCmd_RotateToRelativeTime_1()
{
	this->uniqueID = 0;
}


// From iCommand interface
// Treat like a constructor type thing. It's never called by the Animation control
void cCmd_RotateToRelativeTime_1::Initialize(sMessage startStateData)
{
	// Message contains a vec4, so x, y, z will be the angle to rotate to
	// And w will be the time to move there in
	this->moveToAngle.x = startStateData.vec_fData[0].x;
	this->moveToAngle.y = startStateData.vec_fData[0].y;
	this->moveToAngle.z = startStateData.vec_fData[0].z;
	this->curAngle = this->ourShip->getOrientationEuler();	// These are all in radians
	this->mTimer = (double)startStateData.vec_fData[0].w;
	this->m_bIsDone = false;

	// Angular velocity (or speed of rotation) is the delta theta/delta time
	// So to get the changePerFrame, I need the change in angle (which should be the "distance"
	// between the current angle and the final angle, and divide that value by the total time we want to complete the angle
	// Since I'm using Euler angles for orientation (vec3), that means I also need the "direction" for the vector to change
	// So if the current angle is (0,0,0), and the final angle is (0,90,0) [in radians of course], then the "vector" would be (0,90,0)
	// normalize it, multiply the angular speed... but it's only doing half the angle!

	//// Calculate the velocity to set the ship to so it arrives at the end position in time
	//float distance = glm::distance(this->curAngle, this->moveToAngle);	// "Distance" the ship needs to rotate
	//changePerFrame = this->moveToAngle - this->curAngle;				// Direction the ship needs to rotate
	//changePerFrame = glm::normalize(changePerFrame);					// Normalize the direction so it's a unit vector
	//// speed = distance/time
	//changePerFrame *= distance / (float)this->mTimer;					// So we find the direction and speed the ship needs to move to get to it's point in time
	//// Which we'll have to do in update
	
	return;
}

void cCmd_RotateToRelativeTime_1::AtStart(sMessage startStateData)
{
	this->curAngle = this->ourShip->getOrientationEuler();	// These are all in radians
	// Calculate the velocity to set the ship to so it arrives at the end position in time
	float distance = glm::distance(this->curAngle, this->moveToAngle);	// "Distance" the ship needs to rotate
	changePerSecond = this->moveToAngle - this->curAngle;				// Direction the ship needs to rotate
	changePerSecond = glm::normalize(changePerSecond);					// Normalize the direction so it's a unit vector
	// speed = distance/time
	changePerSecond *= distance / (float)this->mTimer;					// So we find the direction and speed the ship needs to move to get to it's point in time
	// Which we'll have to do in update
	return;
}

void cCmd_RotateToRelativeTime_1::AtEnd(sMessage endStateData)
{
	this->m_bIsDone = true;
	return;
}

void cCmd_RotateToRelativeTime_1::Update(double deltaTime)
{
	if (!this->m_bIsDone)
	{
		this->ourShip->setOrientationEuler(this->ourShip->getOrientationEuler() + this->changePerSecond * (float)deltaTime); // might have to mulitply by deltaTime
		this->mTimer -= deltaTime;
		if (this->mTimer <= 0.0)
		{
			sMessage atEndMessage;
			this->AtEnd(atEndMessage);
		}
	}
	
	return;
}

bool cCmd_RotateToRelativeTime_1::bIsDone(void)
{
	return this->m_bIsDone;
}

int cCmd_RotateToRelativeTime_1::GetUniqueID(void)
{
	return this->uniqueID;
}
