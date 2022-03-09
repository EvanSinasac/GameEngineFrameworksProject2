#pragma once

#include "iCommand.h"

class cCmd_Wait :
	public iCommand
{
private:
	int uniqueID;
public:
	cCmd_Wait();
	virtual ~cCmd_Wait() {};

	// From the iCommand interface:
	virtual void Initialize(sMessage startStateData);
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
	virtual int GetUniqueID(void);

// Specific stuff for this command
	double m_elapsedTime;
	void setTime(double timeToWait);
private:
	double m_timeToWait;
};



class cCmd_WaitFiveSeconds :
	public iCommand
{
private:
	int uniqueID;
public:
	cCmd_WaitFiveSeconds();
	virtual ~cCmd_WaitFiveSeconds() {};

	// From iCommand interface
	virtual void AtStart(sMessage startStateData);
	virtual void AtEnd(sMessage endStateData);
	virtual void Update(double deltaTime);
	virtual bool bIsDone(void);
	virtual int GetUniqueID(void);

// Specific stuff for this command
	double m_elapsedTime;
};