#include "cCmd_FollowWaypoints_InRelativeTime_1.h"
#include "globalThings.h"


cCmd_FollowWaypoints_InRelativeTime_1::cCmd_FollowWaypoints_InRelativeTime_1()
{
	this->uniqueID = 0;
}


// From iCommand interface
// Treat like a constructor type thing. It's never called by the Animation control
void cCmd_FollowWaypoints_InRelativeTime_1::Initialize(sMessage startStateData)
{
	// Message contains a vec4, so x, y, z will be the world position of the first waypoint
	// And w will be the time to move through them
	this->startPoint.x = startStateData.vec_fData[0].x;
	this->startPoint.y = startStateData.vec_fData[0].y;
	this->startPoint.z = startStateData.vec_fData[0].z;
	this->wayPoints.push_back(this->startPoint);
	this->mTimer = (double)startStateData.vec_fData[0].w;
	this->curPos = this->ourShip->getPosition();
	this->m_bIsDone = false;

	// Start by going through the list of waypoints and storing them, all except start and last
	for (unsigned int index = 1; index < startStateData.vec_fData.size(); index++)
	{
		glm::vec3 temp;
		temp.x = startStateData.vec_fData[index].x;
		temp.y = startStateData.vec_fData[index].y;
		temp.z = startStateData.vec_fData[index].z;
		this->wayPoints.push_back(temp);
	}

	//this->finalWaypoint.x = startStateData.vec_fData[startStateData.vec_fData.size() - 1].x;
	//this->finalWaypoint.y = startStateData.vec_fData[startStateData.vec_fData.size() - 1].y;
	//this->finalWaypoint.z = startStateData.vec_fData[startStateData.vec_fData.size() - 1].z;
	//this->wayPoints.push_back(this->finalWaypoint);

	//// To calculate the total distance (and speed we need to move along the waypoints) we need to find the distance from the ship's current
	//// position, and then the distance between all the waypoints
	//float totalDistance = 0.0f;
	//totalDistance += glm::distance(this->curPos, this->startPoint);
	//for (unsigned int index = 0; index < this->wayPoints.size() - 1; index++)
	//{
	//	totalDistance += glm::distance(this->wayPoints[index], this->wayPoints[index + 1]);
	//}

	//this->speedToMoveAt = totalDistance / (float)this->mTimer;

	//// Calculate the velocity to set the ship to so it arrives at the end position in time
	//glm::vec3 direction = this->startPoint - this->curPos;			// Direction the ship needs to move
	//direction = glm::normalize(direction);							// Normalize the direction so it's a unit vector
	//// speed = distance/time
	//direction *= this->speedToMoveAt;					// So we find the direction and speed the ship needs to move to get to it's point in time
	//
	// Can't do this if I want to be able to run commands sequentially...
	//this->ourShip->setVelocity(direction);

	this->currentWaypoint = 0;
	this->numberOfWaypoints = this->wayPoints.size();

	return;
}

void cCmd_FollowWaypoints_InRelativeTime_1::AtStart(sMessage startStateData)
{
	this->curPos = this->ourShip->getPosition();

	// To calculate the total distance (and speed we need to move along the waypoints) we need to find the distance from the ship's current
	// position, and then the distance between all the waypoints
	float totalDistance = 0.0f;
	totalDistance += glm::distance(this->curPos, this->startPoint);
	for (unsigned int index = 0; index < this->wayPoints.size() - 1; index++)
	{
		totalDistance += glm::distance(this->wayPoints[index], this->wayPoints[index + 1]);
	}

	this->speedToMoveAt = totalDistance / (float)this->mTimer;

	// This is moved out of initialize so the commands can be run sequentially/parallel without breaking the animation controller
	// Calculate the velocity to set the ship to so it arrives at the end position in time
	glm::vec3 direction = this->startPoint - this->curPos;			// Direction the ship needs to move
	direction = glm::normalize(direction);							// Normalize the direction so it's a unit vector
	// speed = distance/time
	direction *= this->speedToMoveAt;					// So we find the direction and speed the ship needs to move to get to it's point in time
	this->ourShip->setVelocity(direction);
	return;
}

void cCmd_FollowWaypoints_InRelativeTime_1::AtEnd(sMessage endStateData)
{
	this->m_bIsDone = true;
	this->ourShip->setVelocity(glm::vec3(0.0f));
	return;
}

void cCmd_FollowWaypoints_InRelativeTime_1::Update(double deltaTime)
{
	if (!this->m_bIsDone)
	{
		this->mTimer -= deltaTime;
		if (this->mTimer <= 0.0)
		{
			sMessage atEndMessage;
			this->AtEnd(atEndMessage);
		}
		// Check if we're at the next waypoint
		if (glm::distance(this->ourShip->getPosition(), this->wayPoints[this->currentWaypoint]) <= 1.0f)
		{
			// If we're at the next waypoint, we need to change the velocity, and update the current waypoint
			// First, figure out the direction of the next velocity
			glm::vec3 direction = this->wayPoints[this->currentWaypoint + 1] - this->wayPoints[this->currentWaypoint];
			direction = glm::normalize(direction);
			direction *= this->speedToMoveAt;
			this->ourShip->setVelocity(direction);
			this->currentWaypoint++;
			if (this->currentWaypoint >= this->numberOfWaypoints - 1)
			{
				/*sMessage atEndMessage;
				this->AtEnd(atEndMessage);*/
				this->currentWaypoint = this->numberOfWaypoints - 2; // safety, shouldn't need this, but just in case we somehow get through all the waypoints before the timer.
			}
		}
	}
	
	return;
}

bool cCmd_FollowWaypoints_InRelativeTime_1::bIsDone(void)
{
	return this->m_bIsDone;
}

int cCmd_FollowWaypoints_InRelativeTime_1::GetUniqueID(void)
{
	return this->uniqueID;
}
