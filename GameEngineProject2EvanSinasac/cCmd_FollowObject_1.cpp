#include "cCmd_FollowObject_1.h"



cCmd_FollowObject_1::cCmd_FollowObject_1()
{
	this->uniqueID = 0;
}


// From iCommand interface
// Treat like a constructor type thing. It's never called by the Animation control
void cCmd_FollowObject_1::Initialize(sMessage startStateData)
{
	// Message x, y, z will be the offset
	// Message w will be the follow distance
	this->m_bIsDone = false;
	this->offset.x = startStateData.vec_fData[0].x;
	this->offset.y = startStateData.vec_fData[0].y;
	this->offset.z = startStateData.vec_fData[0].z;
	this->followDistance = startStateData.vec_fData[0].w;
	// Yeah, setting the maxSpeed might help
	this->maxSpeed = startStateData.vec_fData[1].x;

	return;
}

void cCmd_FollowObject_1::AtStart(sMessage startStateData)
{
	// When we start, we need the following ship to figure out where the target is and start moving towards it
	float curDist = glm::distance(this->ourShip->getPosition(), this->targetShip->getPosition() + this->offset);
	glm::vec3 followDirection = this->targetShip->getPosition() + this->offset - this->ourShip->getPosition();
	followDirection = glm::normalize(followDirection);
	followDirection *= this->maxSpeed;
	this->ourShip->setVelocity(followDirection);
	return;
}

void cCmd_FollowObject_1::AtEnd(sMessage endStateData)
{
	this->m_bIsDone = true;
	return;
}

void cCmd_FollowObject_1::Update(double deltaTime)
{
	if (!this->m_bIsDone)
	{
		float curDist = glm::distance(this->ourShip->getPosition(), this->targetShip->getPosition() + this->offset);
		if (curDist > this->followDistance)
		{
			// Update the velocity
			glm::vec3 followDirection = this->targetShip->getPosition() + this->offset - this->ourShip->getPosition();
			followDirection = glm::normalize(followDirection);
			followDirection *= this->maxSpeed;
			this->ourShip->setVelocity(followDirection);
		}
		else
		{
			// Idk, one of these or do nothing, or don't even check
			this->ourShip->setVelocity(this->targetShip->getVelocity());
			//this->ourShip->setVelocity(glm::vec3(0.0f));
		}
		// Otherwise do nothing
	}

	return;
}

bool cCmd_FollowObject_1::bIsDone(void)
{
	return this->m_bIsDone;
}


int cCmd_FollowObject_1::GetUniqueID(void)
{
	return this->uniqueID;
}