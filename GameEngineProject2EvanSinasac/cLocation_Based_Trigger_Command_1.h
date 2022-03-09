#pragma once

// Really not sure how to do this, so just trying it as a permanent command and seeing if it works

#include "iCommand.h"
#include "GLMCommon.h"
#include "iShip.h"

// This is given the ship that will trigger somethign when it reaches within
// the entered radius from the entered point

class cLocation_Based_Trigger_Command_1 :
	public iCommand
{
private:
	bool m_bIsDone;
	glm::vec3 triggerPoint;
	float triggeringDistance;
	iShip* triggerShip;
	int uniqueID;
public:
	cLocation_Based_Trigger_Command_1();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cLocation_Based_Trigger_Command_1() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
	virtual int GetUniqueID(void);

	void setTriggerShip(iShip* aShip)
	{
		this->triggerShip = aShip;
	}
};