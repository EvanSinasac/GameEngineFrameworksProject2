#include "cSpaceInvader.h"

cSpaceInvader::cSpaceInvader()
{
	this->pTheMeshImUsing = NULL;
}

cSpaceInvader::~cSpaceInvader()
{

}

// ******************************************
// From the iShip interface
void cSpaceInvader::Update(double deltaTime)
{

	return;
}

std::string cSpaceInvader::getName(void)
{
	return this->myShipName;
}

cMesh* cSpaceInvader::getMeshPointer(void)
{
	// Update the mesh location to match the ship location:
	this->pTheMeshImUsing->positionXYZ = this->m_PhysicalProperties.position;
	this->pTheMeshImUsing->orientationXYZ = this->m_PhysicalProperties.orientationEuler;

	return this->pTheMeshImUsing;
}


// *******************************************************************
// From the iMessage interface
bool cSpaceInvader::SetReciever(iMediator* pTheReciever)
{
	this->m_pTheMediator = pTheReciever;
	return true;
}

// Asynchronous (don't return)
bool cSpaceInvader::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "SetBlocks")
	{
		// Set all child blocks to invisible
		for (cMesh* pBlock : this->pTheMeshImUsing->vec_pChildMeshes)
		{
			pBlock->bIsVisible = false;
		}

		unsigned int blockIndex = 0;
		for (unsigned int index = 0; index != theMessage.vec_fData.size(); index++, blockIndex += 2)
		{
			cMesh* pSolid = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 0];
			cMesh* pWireframe = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 1u];

			pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pSolid->bUseWholeObjectDiffuseColour = true;
			pSolid->bIsWireframe = false;
			pSolid->bDontLight = true;
			pSolid->textureNames[0] = "White.bmp";
			pSolid->textureRatios[0] = 1.0f;
			pWireframe->setUniformScale(1.0f);
			pSolid->bIsVisible = true;

			pWireframe->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			pWireframe->bIsWireframe = true;
			pWireframe->bDontLight = true;
			pWireframe->setUniformScale(1.05f);
			pWireframe->bIsVisible = true;

		}//for (unsigned int index = 0

	}//theMessage.command == "SetBlocks"

	return true;
}

// Synchronous (do return with something)
bool cSpaceInvader::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

/*static*/ const std::string cSpaceInvader::SPACE_INVADER_SHIPTYPE_AS_STRING = "SpaceInvader";

std::string cSpaceInvader::getShipTypeAsString(void)
{
	return cSpaceInvader::SPACE_INVADER_SHIPTYPE_AS_STRING;
}

unsigned int cSpaceInvader::getShipTypeAsUInt(void)
{
	return cSpaceInvader::SPACE_INVADER_SHIPTYPE_AS_UINT;
}

// *******************************************************************
// From the iPhysicalObject interface
glm::vec3 cSpaceInvader::getPosition(void)
{
	return this->m_PhysicalProperties.position;
}

void cSpaceInvader::setPosition(const glm::vec3& newPosition)
{
	this->m_PhysicalProperties.position = newPosition;
	return;
}

glm::vec3 cSpaceInvader::getVelocity(void)
{
	return this->m_PhysicalProperties.velocity;
}

void cSpaceInvader::setVelocity(const glm::vec3& newVelocity)
{
	this->m_PhysicalProperties.velocity = newVelocity;
	return;
}

glm::vec3 cSpaceInvader::getAcceleration(void)
{
	return this->m_PhysicalProperties.acceleration;
}

void cSpaceInvader::setAcceleration(const glm::vec3& newAcceleration)
{
	this->m_PhysicalProperties.acceleration = newAcceleration;
	return;
}

glm::vec3 cSpaceInvader::getOrientationEuler(void)
{
	return this->m_PhysicalProperties.orientationEuler;
}

void cSpaceInvader::setOrientationEuler(const glm::vec3& newOrientationEuler)
{
	this->m_PhysicalProperties.orientationEuler = newOrientationEuler;
	return;
}

glm::quat cSpaceInvader::getOrientationQuaternion(void)
{
	return this->m_PhysicalProperties.orientationQuat;
}

void cSpaceInvader::setOrientationQuaternion(const glm::quat& newOrientationQuaternion)
{
	this->m_PhysicalProperties.orientationQuat = newOrientationQuaternion;
	return;
}

sPhysicalProps cSpaceInvader::getPhysicalProperties(void)
{
	return this->m_PhysicalProperties;
}

void cSpaceInvader::setPhysicalProperties(const sPhysicalProps& newPhysProps)
{
	this->m_PhysicalProperties = newPhysProps;
	return;
}

