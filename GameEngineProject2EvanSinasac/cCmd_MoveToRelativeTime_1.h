#pragma once

#include "iCommand.h"
#include "GLMCommon.h"
#include "iShip.h"

// This is given a relative location and time,
//	then calculates the velocity to get there.
// Then it sets the velocity, and a timer, and waits.

class cCmd_MoveToRelativeTime_1 :
	public iCommand
{
private:
	glm::vec3 curPos;
	glm::vec3 moveToPos;
	bool m_bIsDone;
	double mTimer;
	iShip* ourShip;
	int uniqueID;
public:
	cCmd_MoveToRelativeTime_1();
	// Because polymorphic (virtual) inheritance, need virtual destructor
	virtual ~cCmd_MoveToRelativeTime_1() {};

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