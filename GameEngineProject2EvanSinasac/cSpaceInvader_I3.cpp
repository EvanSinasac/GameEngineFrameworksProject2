#include "cSpaceInvader_I3.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "globalThings.h"

cSpaceInvader_I3::cSpaceInvader_I3()
{
	this->pTheMeshImUsing = NULL;
	form = true;
	this->exploding1 = false;
	this->exploding2 = false;
	this->explosionTimer1 = 1.0f;
	this->explosionTimer2 = 1.0f;
	this->bombTimer = 10.0f;
}

cSpaceInvader_I3::~cSpaceInvader_I3()
{

}

// ******************************************
// From the iShip interface
void cSpaceInvader_I3::Update(double deltaTime)
{
	if (::canWeMove)
	{
		bombTimer -= deltaTime;
		if (bombTimer <= 0.0f)
		{
			bombTimer = 10.0f;
			srand(time(NULL));
			int random = rand() % 2;
			if (random == 0) // 1/2
			{
				// Make a bomb
				sMessage invBomb;
				invBomb.command = "Space_Invader_Bomb";
				invBomb.vec_sData.push_back("Bomb");
				invBomb.vec_fData.push_back(glm::vec4(this->getPosition(), 1.0f));

				sMessage setBlocks;
				setBlocks.command = "SetBlocks";
				setBlocks.vec_fData.push_back(glm::vec4(100.0f, 100.0f, 100.0f, 100.0f));

				iShip* bomb = ::pShipYard->makeAShip(invBomb);
				bomb->RecieveMessage(setBlocks);
				::g_pEntityOrganizer->addShip(bomb);

			}
		}
		if (exploding1)
		{
			explosionTimer1 -= deltaTime;
			if (explosionTimer1 <= 0.0f)
			{
				exploding1 = false;
				exploding2 = true;
				sMessage explosion;
				explosion.command = "SetExplosion2";
				this->RecieveMessage(explosion);
				::speedOfTime += 0.1f;
			}
		}

		if (exploding2)
		{
			explosionTimer2 -= deltaTime;
			if (explosionTimer2 <= 0.0f)
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
				delete(this);
			}
		}
	}
	
	return;
}

std::string cSpaceInvader_I3::getName(void)
{
	return this->myShipName;
}

cMesh* cSpaceInvader_I3::getMeshPointer(void)
{
	// Update the mesh location to match the ship location:
	this->pTheMeshImUsing->positionXYZ = this->m_PhysicalProperties.position;
	this->pTheMeshImUsing->orientationXYZ = this->m_PhysicalProperties.orientationEuler;

	return this->pTheMeshImUsing;
}


// *******************************************************************
// From the iMessage interface
bool cSpaceInvader_I3::SetReciever(iMediator* pTheReciever)
{
	this->m_pTheMediator = pTheReciever;
	return true;
}

// Asynchronous (don't return)
bool cSpaceInvader_I3::RecieveMessage(sMessage theMessage)
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

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, (153.0f / 255.0f), 1.0f, 1.0f);
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

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, (153.0f / 255.0f), 1.0f, 1.0f);
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

	if (theMessage.command == "TimeToMove")
	{
		if (!this->exploding1 && !this->exploding2)
		{
			this->setPosition(this->getPosition() + glm::vec3(::moveStep, 0.0f, 0.0f));
			sMessage changeMessage;
			if (form)
			{
				changeMessage.command = "SetBlocks2";
				form = false;
			}
			else
			{
				changeMessage.command = "SetBlocks";
				form = true;
			}
			changeMessage.vec_fData.push_back(glm::vec4(100.0f, 100.0f, 100.0f, 100.0f));
			this->RecieveMessage(changeMessage);
		}
		return true;
	}
	if (theMessage.command == "MoveDown")
	{
		this->setPosition(this->getPosition() - glm::vec3(0.0f, 100.0f, 0.0f));
		return true;
	}

	if (theMessage.command == "Explode")
	{
		exploding1 = true;
		sMessage explosion;
		explosion.command = "SetExplosion1";
		this->RecieveMessage(explosion);
		return true;
	}

	if (theMessage.command == "SetExplosion1")
	{
		// Set all child blocks to invisible
		for (cMesh* pBlock : this->pTheMeshImUsing->vec_pChildMeshes)
		{
			pBlock->bIsVisible = false;
		}

		unsigned int blockIndex = 0;

		this->pTheMeshImUsing->bIsVisible = false;

		//unsigned int blockIndex2 = 0;
		for (unsigned int y = 0; y < eY; y++)
		{
			for (unsigned int x = 0; x < eX; x++)
			{
				cMesh* pSolid = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 0];
				cMesh* pWireframe = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 1u];
				//std::cout << this->blockPose1[x][y];
				if (this->explodeBlock1[x][y] == "X")
				{
					pSolid->positionXYZ.x = 0.0f + (10.0f * x);
					pSolid->positionXYZ.y = 0.0f - (10.0f * y);
					pSolid->positionXYZ.z = 20.0f;
					pWireframe->positionXYZ.x = 0.0f + (10.0f * x);
					pWireframe->positionXYZ.y = 0.0f - (10.0f * y);
					pWireframe->positionXYZ.z = 20.0f;

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.8f, 0.2f, 1.0f);
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
	}

	if (theMessage.command == "SetExplosion2")
	{
		// Set all child blocks to invisible
		for (cMesh* pBlock : this->pTheMeshImUsing->vec_pChildMeshes)
		{
			pBlock->bIsVisible = false;
		}

		unsigned int blockIndex = 0;

		this->pTheMeshImUsing->bIsVisible = false;

		//unsigned int blockIndex2 = 0;
		for (unsigned int y = 0; y < eY; y++)
		{
			for (unsigned int x = 0; x < eX; x++)
			{
				cMesh* pSolid = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 0];
				cMesh* pWireframe = this->pTheMeshImUsing->vec_pChildMeshes[blockIndex + 1u];
				//std::cout << this->blockPose1[x][y];
				if (this->explodeBlock2[x][y] == "X")
				{
					pSolid->positionXYZ.x = 0.0f + (10.0f * x);
					pSolid->positionXYZ.y = 0.0f - (10.0f * y);
					pSolid->positionXYZ.z = 20.0f;
					pWireframe->positionXYZ.x = 0.0f + (10.0f * x);
					pWireframe->positionXYZ.y = 0.0f - (10.0f * y);
					pWireframe->positionXYZ.z = 20.0f;

					pSolid->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.8f, 0.2f, 1.0f);
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
	}

	return true;
}

// Synchronous (do return with something)
bool cSpaceInvader_I3::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return true;
}

/*static*/ const std::string cSpaceInvader_I3::SPACE_INVADER_SHIPTYPE_AS_STRING = "I3";

std::string cSpaceInvader_I3::getShipTypeAsString(void)
{
	return cSpaceInvader_I3::SPACE_INVADER_SHIPTYPE_AS_STRING;
}

unsigned int cSpaceInvader_I3::getShipTypeAsUInt(void)
{
	return cSpaceInvader_I3::SPACE_INVADER_SHIPTYPE_AS_UINT;
}

// *******************************************************************
// From the iPhysicalObject interface
glm::vec3 cSpaceInvader_I3::getPosition(void)
{
	return this->m_PhysicalProperties.position;
}

void cSpaceInvader_I3::setPosition(const glm::vec3& newPosition)
{
	this->m_PhysicalProperties.position = newPosition;
	return;
}

glm::vec3 cSpaceInvader_I3::getVelocity(void)
{
	return this->m_PhysicalProperties.velocity;
}

void cSpaceInvader_I3::setVelocity(const glm::vec3& newVelocity)
{
	this->m_PhysicalProperties.velocity = newVelocity;
	return;
}

glm::vec3 cSpaceInvader_I3::getAcceleration(void)
{
	return this->m_PhysicalProperties.acceleration;
}

void cSpaceInvader_I3::setAcceleration(const glm::vec3& newAcceleration)
{
	this->m_PhysicalProperties.acceleration = newAcceleration;
	return;
}

glm::vec3 cSpaceInvader_I3::getOrientationEuler(void)
{
	return this->m_PhysicalProperties.orientationEuler;
}

void cSpaceInvader_I3::setOrientationEuler(const glm::vec3& newOrientationEuler)
{
	this->m_PhysicalProperties.orientationEuler = newOrientationEuler;
	return;
}

glm::quat cSpaceInvader_I3::getOrientationQuaternion(void)
{
	return this->m_PhysicalProperties.orientationQuat;
}

void cSpaceInvader_I3::setOrientationQuaternion(const glm::quat& newOrientationQuaternion)
{
	this->m_PhysicalProperties.orientationQuat = newOrientationQuaternion;
	return;
}

sPhysicalProps cSpaceInvader_I3::getPhysicalProperties(void)
{
	return this->m_PhysicalProperties;
}

void cSpaceInvader_I3::setPhysicalProperties(const sPhysicalProps& newPhysProps)
{
	this->m_PhysicalProperties = newPhysProps;
	return;
}


bool cSpaceInvader_I3::loadBlockPose1(std::string fileName)
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

bool cSpaceInvader_I3::loadBlockPose2(std::string fileName)
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

	std::cout << std::endl;

	return true;
}

bool cSpaceInvader_I3::loadExplosion1(std::string fileName)
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
	for (unsigned int y = 0; y < eY; y++)
	{
		for (unsigned int x = 0; x < eX; x++)
		{
			nextLetter = theFile.get();
			this->explodeBlock1[x][y] = nextLetter;
		}
		theFile.get();	//newline
	}
	return true;
}

bool cSpaceInvader_I3::loadExplosion2(std::string fileName)
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
	for (unsigned int y = 0; y < eY; y++)
	{
		for (unsigned int x = 0; x < eX; x++)
		{
			nextLetter = theFile.get();
			this->explodeBlock2[x][y] = nextLetter;
		}
		theFile.get();	//newline
	}
	return true;
}


