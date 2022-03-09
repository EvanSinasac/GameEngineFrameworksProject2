#include "cLocation_Based_Trigger_Command_1.h"
#include "globalThings.h"


cLocation_Based_Trigger_Command_1::cLocation_Based_Trigger_Command_1()
{
	this->uniqueID = 0;
}


// From iCommand interface
// Treat like a constructor type thing. It's never called by the Animation control
void cLocation_Based_Trigger_Command_1::Initialize(sMessage startStateData)
{
	// Message contains a vec4, so x, y, z will be the world position that will be the trigger location
	// And w will be the distance from the world position the ship can be
	this->triggerPoint.x = startStateData.vec_fData[0].x;
	this->triggerPoint.y = startStateData.vec_fData[0].y;
	this->triggerPoint.z = startStateData.vec_fData[0].z;
	this->triggeringDistance = startStateData.vec_fData[0].w;

	return;
}

void cLocation_Based_Trigger_Command_1::AtStart(sMessage startStateData)
{
	
	return;
}

void cLocation_Based_Trigger_Command_1::AtEnd(sMessage endStateData)
{
	this->m_bIsDone = true;
	return;
}

void cLocation_Based_Trigger_Command_1::Update(double deltaTime)
{
	if (glm::distance(this->triggerPoint, this->triggerShip->getPosition()) <= this->triggeringDistance)
	{
		::g_pTheLights->TurnOnLight(0);
	}
	else
	{
		::g_pTheLights->TurnOffLight(0);
	}

	return;
}

bool cLocation_Based_Trigger_Command_1::bIsDone(void)
{
	return this->m_bIsDone;
}

int cLocation_Based_Trigger_Command_1::GetUniqueID(void)
{
	return this->uniqueID;
}