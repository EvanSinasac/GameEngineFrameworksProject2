#pragma once

// This will generate ships for the game
// This is an Abstract Factory:
//	* Returns variations of the iShip interface
// This is also a Builder:
//	* Combines various other things (values or other objects)
//	  and "builds" or "assembles" them and returns some common type

#include "iShip.h"
#include "iMessage.h"
#include "iMediator.h"	// To give to all the ships, etc.

class cShipYard
{
public:
	cShipYard();
	~cShipYard();

	// Factory or Builder method to "make ships"

	void SetMediator(iMediator* pTheMediator);

	// Returns NULL (0) if unknown or error
	iShip* makeAShip(sMessage typeOfShip);

	// Clears error when read
	std::string getLastError(void);

private:
	std::string m_lastError;

	iMediator* m_pTheMediator;

};
