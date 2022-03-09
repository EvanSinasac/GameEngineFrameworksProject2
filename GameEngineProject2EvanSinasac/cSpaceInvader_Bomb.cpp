#include "cSpaceInvader_Bomb.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "globalThings.h"

cSpaceInvader_Bomb::cSpaceInvader_Bomb()
{
	this->pTheMeshImUsing = NULL;
	form = 0;
	this->formTimer = 1.0f;
	this->die = false;
}

cSpaceInvader_Bomb::~cSpaceInvader_Bomb()
{

}

// ******************************************
// From the iShip interface
void cSpaceInvader_Bomb::Update(double deltaTime)
{
		this->formTimer -= deltaTime;
		if (this->formTimer <= 0.0f)
		{
			sMessage whichForm;
			switch (form)
			{
			case 0:
				whichForm.command = "SetBlocks";
				break;
			case 1:
				whichForm.command = "SetBlocks2";
				break;
			case 2:
				whichForm.command = "SetBlocks3";
				break;
			default:
				whichForm.command = "SetBlocks";
			}
			form++;
			if (form > 2)
				form = 1;
			formTimer = 1.0f;
			this->RecieveMessage(whichForm);
		}


		this->setPosition(glm::vec3(this->getPosition().x, this->getPosition().y - 200.0f * deltaTime, this->getPosition().z));
		if (this->getPosition().y <= 0.0f && !this->die)
		{
			std::vector<iShip*>* p_MoveTheShips = ::g_pEntityOrganizer->get_pVecShipsForUpdate();
			for (unsigned int index = 0; index != p_MoveTheShips->size(); index++)
			{
				if ((*p_MoveTheShips)[index] == this)
				{
					(*p_MoveTheShips).erase((*p_MoveTheShips).begin() + index);
					break;
				}
			}
			//delete(this);
			this->die = true;
		}

		float xDistance = this->getPosition().x - ::g_pPlayer->getPosition().x;
		float yDistance = this->getPosition().y - ::g_pPlayer->getPosition().y;
		if (xDistance >= 10.0f
			&& xDistance <= 200.0f
			&& yDistance >= -50.0f
			&& yDistance <= 50.0f)
		{
			sMessage explode;
			explode.command = "Explode";
			this->RecieveMessage(explode);
			::g_pPlayer->RecieveMessage(explode);
			this->die = true;
		}

		if (this->die)
		{
			sMessage hide;
			hide.command = "Hide";
			this->RecieveMessage(hide);
			//delete(this);
		}
	
	
	return;
}

std::string cSpaceInvader_Bomb::getName(void)
{
	return this->myShipName;
}

cMesh* cSpaceInvader_Bomb::getMeshPointer(void)
{
	// Update the mesh location to match the ship location:
	this->pTheMeshImUsing->positionXYZ = this->m_PhysicalProperties.position;
	this->pTheMeshImUsing->orientationXYZ = this->m_PhysicalProperties.orientationEuler;

	return this->pTheMeshImUsing;
}


// *******************************************************************
// From the iMessage interface
bool cSpaceInvader_Bomb::SetReciever(iMediator* pTheReciever)
{
	this->m_pTheMediator = pTheReciever;
	return true;
}

// Asynchronous (don't return)
bool cSpaceInvader_Bomb::RecieveMessage(sMessage theMessage)
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

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
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

	if (theMessage.command == "SetBlocks2")
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
				if (this->blockPose2[x][y] == "X")
				{
					pSolid->positionXYZ.x = 0.0f + (10.0f * x);
					pSolid->positionXYZ.y = 0.0f - (10.0f * y);
					pSolid->positionXYZ.z = 20.0f;
					pWireframe->positionXYZ.x = 0.0f + (10.0f * x);
					pWireframe->positionXYZ.y = 0.0f - (10.0f * y);
					pWireframe->positionXYZ.z = 20.0f;

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
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
		}
		return true;
	}//theMessage.command == "SetBlocks2"

	if (theMessage.command == "SetBlocks3")
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
				if (this->blockPose3[x][y] == "X")
				{
					pSolid->positionXYZ.x = 0.0f + (10.0f * x);
					pSolid->positionXYZ.y = 0.0f - (10.0f * y);
					pSolid->positionXYZ.z = 20.0f;
					pWireframe->positionXYZ.x = 0.0f + (10.0f * x);
					pWireframe->positionXYZ.y = 0.0f - (10.0f * y);
					pWireframe->positionXYZ.z = 20.0f;

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
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
		}
		return true;
	}//theMessage.command == "SetBlocks3"

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
bool cSpaceInvader_Bomb::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

/*static*/ const std::string cSpaceInvader_Bomb::SPACE_INVADER_SHIPTYPE_AS_STRING = "I3";

std::string cSpaceInvader_Bomb::getShipTypeAsString(void)
{
	return cSpaceInvader_Bomb::SPACE_INVADER_SHIPTYPE_AS_STRING;
}

unsigned int cSpaceInvader_Bomb::getShipTypeAsUInt(void)
{
	return cSpaceInvader_Bomb::SPACE_INVADER_SHIPTYPE_AS_UINT;
}

// *******************************************************************
// From the iPhysicalObject interface
glm::vec3 cSpaceInvader_Bomb::getPosition(void)
{
	return this->m_PhysicalProperties.position;
}

void cSpaceInvader_Bomb::setPosition(const glm::vec3& newPosition)
{
	this->m_PhysicalProperties.position = newPosition;
	return;
}

glm::vec3 cSpaceInvader_Bomb::getVelocity(void)
{
	return this->m_PhysicalProperties.velocity;
}

void cSpaceInvader_Bomb::setVelocity(const glm::vec3& newVelocity)
{
	this->m_PhysicalProperties.velocity = newVelocity;
	return;
}

glm::vec3 cSpaceInvader_Bomb::getAcceleration(void)
{
	return this->m_PhysicalProperties.acceleration;
}

void cSpaceInvader_Bomb::setAcceleration(const glm::vec3& newAcceleration)
{
	this->m_PhysicalProperties.acceleration = newAcceleration;
	return;
}

glm::vec3 cSpaceInvader_Bomb::getOrientationEuler(void)
{
	return this->m_PhysicalProperties.orientationEuler;
}

void cSpaceInvader_Bomb::setOrientationEuler(const glm::vec3& newOrientationEuler)
{
	this->m_PhysicalProperties.orientationEuler = newOrientationEuler;
	return;
}

glm::quat cSpaceInvader_Bomb::getOrientationQuaternion(void)
{
	return this->m_PhysicalProperties.orientationQuat;
}

void cSpaceInvader_Bomb::setOrientationQuaternion(const glm::quat& newOrientationQuaternion)
{
	this->m_PhysicalProperties.orientationQuat = newOrientationQuaternion;
	return;
}

sPhysicalProps cSpaceInvader_Bomb::getPhysicalProperties(void)
{
	return this->m_PhysicalProperties;
}

void cSpaceInvader_Bomb::setPhysicalProperties(const sPhysicalProps& newPhysProps)
{
	this->m_PhysicalProperties = newPhysProps;
	return;
}


bool cSpaceInvader_Bomb::loadBlockPose1(std::string fileName)
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
	//std::cout << "I3 Block Pose 1:" << std::endl;
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

bool cSpaceInvader_Bomb::loadBlockPose2(std::string fileName)
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
	//std::cout << "I3: Block Pose 2" << std::endl;
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			nextLetter = theFile.get();
			this->blockPose2[x][y] = nextLetter;
			//std::cout << this->blockPose2[x][y];
		}
		theFile.get();	//newline
		//std::cout << std::endl;
	}

	//std::cout << std::endl;

	return true;
}

bool cSpaceInvader_Bomb::loadBlockPose3(std::string fileName)
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
	//std::cout << "I3: Block Pose 2" << std::endl;
	for (unsigned int y = 0; y < Y; y++)
	{
		for (unsigned int x = 0; x < X; x++)
		{
			nextLetter = theFile.get();
			this->blockPose3[x][y] = nextLetter;
			//std::cout << this->blockPose2[x][y];
		}
		theFile.get();	//newline
	//	std::cout << std::endl;
	}

	//std::cout << std::endl;

	return true;
}
