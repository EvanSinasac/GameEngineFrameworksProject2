#pragma once

#include "iMessage.h"

class iCommand
{
public:
	virtual ~iCommand() {};

	// Called by whoever instantiated this command.
	// It's +NOT+ called by the groups or animation control.
	// (It's basically stuff you might send via a constructor)
	virtual void Initialize(sMessage startStateData) = 0; 

	// Called once when created
	virtual void AtStart(sMessage startStateData) = 0;
	// Called once when done
	virtual void AtEnd(sMessage endStateData) = 0;

	// Called every frame
	virtual void Update(double deltaTime) = 0;

	// When command is finished, this returns true;
	virtual bool bIsDone(void) = 0;

	virtual int GetUniqueID(void) = 0;
};
