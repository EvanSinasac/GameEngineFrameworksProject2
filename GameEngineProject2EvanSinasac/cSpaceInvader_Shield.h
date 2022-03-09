#pragma once

// Was used for testing

#include "cSpaceInvader.h"
#include <string>
#include "cMesh.h"

class cSpaceInvader_Shield :
	public cSpaceInvader
{
public:
	cSpaceInvader_Shield();
	virtual ~cSpaceInvader_Shield();

	// ******************************************
	// From the iShip interface
	virtual void Update(double deltaTime);
	virtual std::string getName(void);
	virtual cMesh* getMeshPointer(void);
	// ******************************************

	// *******************************************************************
	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);
	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);
	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
	// These are used by the builder or anyone else to determine the type of ship.
	virtual std::string getShipTypeAsString(void);
	virtual unsigned int getShipTypeAsUInt(void);
	// *******************************************************************

	// *******************************************************************
	// From the iPhysicalObject interface
	virtual glm::vec3 getPosition(void);
	virtual void setPosition(const glm::vec3& newPosition);
	virtual glm::vec3 getVelocity(void);
	virtual void setVelocity(const glm::vec3& newVelocity);
	virtual glm::vec3 getAcceleration(void);
	virtual void setAcceleration(const glm::vec3& newAcceleration);
	virtual glm::vec3 getOrientationEuler(void);
	virtual void setOrientationEuler(const glm::vec3& newOrientationEuler);
	virtual glm::quat getOrientationQuaternion(void);
	virtual void setOrientationQuaternion(const glm::quat& newOrientationQuaternion);
	virtual sPhysicalProps getPhysicalProperties(void);
	virtual void setPhysicalProperties(const sPhysicalProps& newPhysProps);
	// *******************************************************************

	std::string myShipName;

	cMesh* pTheMeshImUsing;

	static const unsigned int MAX_NUMBER_OF_CHILD_BLOCKS = 500;

	static const unsigned int X = 13;
	static const unsigned int Y = 10;

	bool loadBlockPose1(std::string fileName);

private:
	iMediator* m_pTheMediator;

	static const std::string SPACE_INVADER_SHIPTYPE_AS_STRING;// = "SpaceInvader";
	static const unsigned int SPACE_INVADER_SHIPTYPE_AS_UINT = 15;

	sPhysicalProps m_PhysicalProperties;

	std::string blockPose1[X][Y];

};