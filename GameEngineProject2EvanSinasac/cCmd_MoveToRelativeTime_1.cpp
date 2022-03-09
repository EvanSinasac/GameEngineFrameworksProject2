#include "cCmd_MoveToRelativeTime_1.h"



cCmd_MoveToRelativeTime_1::cCmd_MoveToRelativeTime_1()
{
	this->uniqueID = 0;
}


// From iCommand interface
// Treat like a constructor type thing. It's never called by the Animation control
void cCmd_MoveToRelativeTime_1::Initialize(sMessage startStateData)
{
	// Message contains a vec4, so x, y, z will be the world position to move to
	// And w will be the time to move there in
	this->moveToPos.x = startStateData.vec_fData[0].x;
	this->moveToPos.y = startStateData.vec_fData[0].y;
	this->moveToPos.z = startStateData.vec_fData[0].z;
	this->curPos = this->ourShip->getPosition();
	this->mTimer = (double)startStateData.vec_fData[0].w;
	this->m_bIsDone = false;

	//// Calculate the velocity to set the ship to so it arrives at the end position in time
	//float distance = glm::distance(this->curPos, this->moveToPos);	// Distance the ship needs to move
	//glm::vec3 direction = this->moveToPos - this->curPos;			// Direction the ship needs to move
	//direction = glm::normalize(direction);							// Normalize the direction so it's a unit vector
	//// speed = distance/time
	//direction *= distance / (float)this->mTimer;					// So we find the direction and speed the ship needs to move to get to it's point in time
	//this->ourShip->setVelocity(direction);

	return;
}

void cCmd_MoveToRelativeTime_1::AtStart(sMessage startStateData)
{
	this->curPos = this->ourShip->getPosition();
	// Moved out of initialize so the command can be used sequentially/in parallel
	// Calculate the velocity to set the ship to so it arrives at the end position in time
	float distance = glm::distance(this->curPos, this->moveToPos);	// Distance the ship needs to move
	glm::vec3 direction = this->moveToPos - this->curPos;			// Direction the ship needs to move
	direction = glm::normalize(direction);							// Normalize the direction so it's a unit vector
	// speed = distance/time
	direction *= distance / (float)this->mTimer;					// So we find the direction and speed the ship needs to move to get to it's point in time
	this->ourShip->setVelocity(direction);
	return;
}

void cCmd_MoveToRelativeTime_1::AtEnd(sMessage endStateData)
{
	this->m_bIsDone = true;
	this->ourShip->setVelocity(glm::vec3(0.0f));
	return;
}

void cCmd_MoveToRelativeTime_1::Update(double deltaTime)
{
	if (!this->m_bIsDone)
	{
		this->mTimer -= deltaTime;
		if (this->mTimer <= 0.0)
		{
			sMessage atEndMessage;
			this->AtEnd(atEndMessage);
		}
	}
	
	return;
}

bool cCmd_MoveToRelativeTime_1::bIsDone(void)
{
	return this->m_bIsDone;
}

int cCmd_MoveToRelativeTime_1::GetUniqueID(void)
{
	return this->uniqueID;
}
