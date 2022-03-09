#pragma once

#include "iCommand.h"
#include "GLMCommon.h"
#include "iShip.h"

// This is given a relative angle and time,
//	then calculates the amount to change the angle per update to get there.
// Then it sets the rotation speed, and a timer, and waits.
// I think I can basically treat this the same as the move to, but using orientation instead

class cCmd_RotateToRelativeTime_1 :
	public iCommand
{
private:
	glm::vec3 curAngle;
	glm::vec3 moveToAngle;
	glm::vec3 changePerSecond;
	bool m_bIsDone;
	double mTimer;
	iShip* ourShip;
	int uniqueID;
public:
	cCmd_RotateToRelativeTime_1();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_RotateToRelativeTime_1() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
	virtual int GetUniqueID(void);

	void setShip(iShip* playerShip)
	{
		this->ourShip = playerShip;
	}
};