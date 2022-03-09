#include "cCmd_Wait.h"

//#include <iostream>

cCmd_Wait::cCmd_Wait()
{
	this->m_elapsedTime = 0.0f;
	this->uniqueID = 0;
}

void cCmd_Wait::setTime(double timeToWait)
{
	this->m_timeToWait = timeToWait;
	return;
}


// From iCommand interface

// Treat like a constructor type thing. It's never called by the Animation control
// 
void cCmd_Wait::Initialize(sMessage startStateData)
{
	this->m_timeToWait = (double)startStateData.vec_fData[0].x;
	return;
}

void cCmd_Wait::AtStart(sMessage startStateData)
{
	return;
}

void cCmd_Wait::AtEnd(sMessage endStateData)
{
	return;
}

// Called every frame
void cCmd_Wait::Update(double deltaTime)
{
	this->m_elapsedTime += deltaTime;
	return;
}

bool cCmd_Wait::bIsDone(void)
{
	// 5 seconds yet??
	if ( this->m_elapsedTime >= this->m_timeToWait )
	{
		return true;
	}
	return false;
}

int cCmd_Wait::GetUniqueID(void)
{
	return this->uniqueID;
}







cCmd_WaitFiveSeconds::cCmd_WaitFiveSeconds()
{
	this->m_elapsedTime = 0.0f;
	this->uniqueID = 0;
}

// From iCommand interface
void cCmd_WaitFiveSeconds::AtStart(sMessage startStateData)
{
//	std::cout << "wait 5 seconds AtStart()" << std::endl;
	return;
}

void cCmd_WaitFiveSeconds::AtEnd(sMessage endStateData)
{
//	std::cout << "wait 5 seconds AtEnd()" << std::endl;
	return;
}

// Called every frame
void cCmd_WaitFiveSeconds::Update(double deltaTime)
{
	this->m_elapsedTime += deltaTime;
	return;
}

bool cCmd_WaitFiveSeconds::bIsDone(void)
{
	// 5 seconds yet??
	if (this->m_elapsedTime >= 5.0)
	{
//		std::cout << "wait 10 seconds is done" << std::endl;
		return true;
	}
	return false;
}

int cCmd_WaitFiveSeconds::GetUniqueID(void)
{
	return this->uniqueID;
}
