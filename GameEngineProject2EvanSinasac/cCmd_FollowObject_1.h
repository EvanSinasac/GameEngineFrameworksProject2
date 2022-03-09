#pragma once

#include "iCommand.h"
#include "GLMCommon.h"
#include "iShip.h"

// This is given the two ships, the ship that will be following
// And the ship that is being followed
// as well as the offset between the ships and the maxSpeed and following distance from the ship

class cCmd_FollowObject_1 :
	public iCommand
{
private:
	bool m_bIsDone;
	glm::vec3 offset;
	float followDistance;
	float maxSpeed;
	iShip* ourShip;
	iShip* targetShip;
	int uniqueID;
public:
	cCmd_FollowObject_1();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_FollowObject_1() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
	virtual int GetUniqueID(void);

	void setOurShip(iShip* aShip)
	{
		this->ourShip = aShip;
	}
	void setTargetShip(iShip* bShip)
	{
		this->targetShip = bShip;
	}
};