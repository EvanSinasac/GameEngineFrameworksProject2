#include "cSpaceInvader_UFO.h"
#include <sstream>
#include <fstream>
#include <iostream>

cSpaceInvader_UFO::cSpaceInvader_UFO()
{
	this->pTheMeshImUsing = NULL;
}

cSpaceInvader_UFO::~cSpaceInvader_UFO()
{

}

// ******************************************
// From the iShip interface
void cSpaceInvader_UFO::Update(double deltaTime)
{

	return;
}

std::string cSpaceInvader_UFO::getName(void)
{
	return this->myShipName;
}

cMesh* cSpaceInvader_UFO::getMeshPointer(void)
{
	// Update the mesh location to match the ship location:
	this->pTheMeshImUsing->positionXYZ = this->m_PhysicalProperties.position;
	this->pTheMeshImUsing->orientationXYZ = this->m_PhysicalProperties.orientationEuler;

	return this->pTheMeshImUsing;
}


// *******************************************************************
// From the iMessage interface
bool cSpaceInvader_UFO::SetReciever(iMediator* pTheReciever)
{
	this->m_pTheMediator = pTheReciever;
	return true;
}

// Asynchronous (don't return)
bool cSpaceInvader_UFO::RecieveMessage(sMessage theMessage)
{
	if (theMessage.command == "SetBlocks")
	{
		// Set all child blocks to invisible
		for (cMesh* pBlock : this->pTheMeshImUsing->vec_pChildMeshes)
		{
			pBlock->bIsVisible = false;
		}

		unsigned int blockIndex = 0;

		this->pTheMeshImUsing->bIsVisible = false;

		//unsigned int blockIndex2 = 0;
		for (unsigned int y = 0; y < Y; y++)
		{
			for (unsigned int x = 0; x < X; x++)
			{
				cMesh* pSolid = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 0];
				cMesh* pWireframe = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 1u];
				//std::cout << this->blockPose1[x][y];
				if (this->blockPose1[x][y] == "X")
				{
					pSolid->positionXYZ.x = 0.0f + (10.0f * x);
					pSolid->positionXYZ.y = 0.0f - (10.0f * y);
					pSolid->positionXYZ.z = 20.0f;
					pWireframe->positionXYZ.x = 0.0f + (10.0f * x);
					pWireframe->positionXYZ.y = 0.0f - (10.0f * y);
					pWireframe->positionXYZ.z = 20.0f;

					pSolid->wholeObjectDiffuseRGBA = glm::vec4((204.0f / 255.0f), 0.0f, (204.0f / 255.0f), 1.0f);
					pSolid->bUseWholeObjectDiffuseColour = true;
					pSolid->bIsWireframe = false;
					pSolid->bDontLight = true;
					pSolid->textureNames[0] = "White.bmp";
					pSolid->textureRatios[0] = 1.0f;
					pSolid->setUniformScale(1.0f);
					pSolid->bIsVisible = true;

					pWireframe->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					pWireframe->bIsWireframe = true;
					pWireframe->bDontLight = true;
					pWireframe->setUniformScale(1.05f);
					pWireframe->bIsVisible = true;

					blockIndex += 2;
				}
			}
			//std::cout << std::endl;
		}
		return true;
	}//theMessage.command == "SetBlocks"

	if (theMessage.command == "Attack")
	{
		this->setPosition(theMessage.vec_fData[0]);
		sMessage setBlocks;
		setBlocks.command = "SetBlocks";
		this->RecieveMessage(setBlocks);
		return true;
	}

	if (theMessage.command == "Hide")
	{
		for (cMesh* pBlock : this->pTheMeshImUsing->vec_pChildMeshes)
		{
			pBlock->bIsVisible = false;
		}
		this->pTheMeshImUsing->bIsVisible = false;

		return true;
	}

	return true;
}

// Synchronous (do return with something)
bool cSpaceInvader_UFO::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

/*static*/ const std::string cSpaceInvader_UFO::SPACE_INVADER_SHIPTYPE_AS_STRING = "UFO";

std::string cSpaceInvader_UFO::getShipTypeAsString(void)
{
	return cSpaceInvader_UFO::SPACE_INVADER_SHIPTYPE_AS_STRING;
}

unsigned int cSpaceInvader_UFO::getShipTypeAsUInt(void)
{
	return cSpaceInvader_UFO::SPACE_INVADER_SHIPTYPE_AS_UINT;
}

// *******************************************************************
// From the iPhysicalObject interface
glm::vec3 cSpaceInvader_UFO::getPosition(void)
{
	return this->m_PhysicalProperties.position;
}

void cSpaceInvader_UFO::setPosition(const glm::vec3& newPosition)
{
	this->m_PhysicalProperties.position = newPosition;
	return;
}

glm::vec3 cSpaceInvader_UFO::getVelocity(void)
{
	return this->m_PhysicalProperties.velocity;
}

void cSpaceInvader_UFO::setVelocity(const glm::vec3& newVelocity)
{
	this->m_PhysicalProperties.velocity = newVelocity;
	return;
}

glm::vec3 cSpaceInvader_UFO::getAcceleration(void)
{
	return this->m_PhysicalProperties.acceleration;
}

void cSpaceInvader_UFO::setAcceleration(const glm::vec3& newAcceleration)
{
	this->m_PhysicalProperties.acceleration = newAcceleration;
	return;
}

glm::vec3 cSpaceInvader_UFO::getOrientationEuler(void)
{
	return this->m_PhysicalProperties.orientationEuler;
}

void cSpaceInvader_UFO::setOrientationEuler(const glm::vec3& newOrientationEuler)
{
	this->m_PhysicalProperties.orientationEuler = newOrientationEuler;
	return;
}

glm::quat cSpaceInvader_UFO::getOrientationQuaternion(void)
{
	return this->m_PhysicalProperties.orientationQuat;
}

void cSpaceInvader_UFO::setOrientationQuaternion(const glm::quat& newOrientationQuaternion)
{
	this->m_PhysicalProperties.orientationQuat = newOrientationQuaternion;
	return;
}

sPhysicalProps cSpaceInvader_UFO::getPhysicalProperties(void)
{
	return this->m_PhysicalProperties;
}

void cSpaceInvader_UFO::setPhysicalProperties(const sPhysicalProps& newPhysProps)
{
	this->m_PhysicalProperties = newPhysProps;
	return;
}


bool cSpaceInvader_UFO::loadBlockPose1(std::string fileName)
{
	std::stringstream ss;
	//ss << SOLUTION_DIR << "\\GameEngineFinalEvanSinasac\\";
	ss << fileName;
	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		return false;
	}
	std::string nextLetter;
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			nextLetter = theFile.get();
			this->blockPose1[x][y] = nextLetter;
			//std::cout << this->blockPose1[x][y];
		}
		//std::cout << std::endl;
		theFile.get();	//newline
	}

	//std::cout << std::endl;

	return true;
}

