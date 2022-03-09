#pragma once
#include "iMessage.h"
#include "iPhysicalObject.h"
#include "cMesh.h"

// Interface for the aliens from Space Invaders

// Pure virtual class

class iShip :
	public iMessage,
	public iPhysicalObject
{
public:
	virtual ~iShip() {};

	// Called every frame
	virtual void Update(double deltaTime) = 0;

	// Friendly name for this alien
	virtual std::string getName(void) = 0;

	// Returns the cMesh object to draw for the draw call
	// Returns NULL if there's nothing to draw (maybe it's invisible or something)
	virtual cMesh* getMeshPointer(void) = 0;

	// Used by builder to determine type of alien
	virtual std::string getShipTypeAsString(void) = 0;
	virtual unsigned int getShipTypeAsUInt(void) = 0;
};