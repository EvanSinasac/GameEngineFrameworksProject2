#include "cShipYard.h"

#include "cSpaceInvader.h"
#include "cSpaceInvader_I1.h"
#include "cSpaceInvader_I2.h"
#include "cSpaceInvader_I3.h"
#include "cSpaceInvader_Base.h"
#include "cSpaceInvader_Shield.h"
#include "cSpaceInvader_UFO.h"
#include "cSpaceInvader_Missile.h"
#include "cSpaceInvader_Bomb.h"
// For the matrix transformations
//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include <sstream>

// Types of ships:
// 
// command			| vec_sData[0]   | vec_fData[0]      | vec_fData[1]
// "Bomber"			| name of ship   | starting position | RGBA colour
// "Command"		| name of player | (always at origin)|
// "Gas Harvester"	| name of ship   | starting position | RGBA colour

cShipYard::cShipYard()
{
	this->m_pTheMediator = NULL;
}

cShipYard::~cShipYard()
{
}

void cShipYard::SetMediator(iMediator* pTheMediator)
{
	this->m_pTheMediator = pTheMediator;
	return;
}



// Factory or Builder method to "make ships"
iShip* cShipYard::makeAShip(sMessage typeOfShip)
{
	// Unused
	if (typeOfShip.command == "Space_Invader_Basic")
	{
		//		iShip* pShipIMade = new cBomberShip();
		cSpaceInvader* pShipIMade = new cSpaceInvader();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		// early exit
		return pShipIMade;
	}
	
	if (typeOfShip.command == "Space_Invader_I1")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_I1* pShipIMade = new cSpaceInvader_I1();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_I1::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_1.txt";
		pShipIMade->loadBlockPose1(ss.str());

		ss.str("");
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_2.txt";
		pShipIMade->loadBlockPose2(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_1.txt";
		pShipIMade->loadExplosion1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_2.txt";
		pShipIMade->loadExplosion2(ss.str());

		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_I2")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_I2* pShipIMade = new cSpaceInvader_I2();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_I2::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_1.txt";
		pShipIMade->loadBlockPose1(ss.str());

		ss.str("");
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_2.txt";
		pShipIMade->loadBlockPose2(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_1.txt";
		pShipIMade->loadExplosion1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_2.txt";
		pShipIMade->loadExplosion2(ss.str());

		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_I3")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_I3* pShipIMade = new cSpaceInvader_I3();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_I3::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_1.txt";
		pShipIMade->loadBlockPose1(ss.str());

		ss.str("");
		ss << "SpaceInvader_" << pShipIMade->myShipName << "_block_pose_2.txt";
		pShipIMade->loadBlockPose2(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_1.txt";
		pShipIMade->loadExplosion1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_2.txt";
		pShipIMade->loadExplosion2(ss.str());

		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_Base")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_Base* pShipIMade = new cSpaceInvader_Base();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = false;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_Base::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = false;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_Base_block.txt";
		pShipIMade->loadBlockPose1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_1.txt";
		pShipIMade->loadExplosion1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Explosion_2.txt";
		pShipIMade->loadExplosion2(ss.str());


		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_Shield")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_Shield* pShipIMade = new cSpaceInvader_Shield();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_Shield::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_Shield_block.txt";
		pShipIMade->loadBlockPose1(ss.str());


		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_UFO")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_UFO* pShipIMade = new cSpaceInvader_UFO();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_UFO::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_UFO_block.txt";
		pShipIMade->loadBlockPose1(ss.str());


		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_Bomb")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_Bomb* pShipIMade = new cSpaceInvader_Bomb();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_Missile::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_Bomb_pose_1.txt";
		pShipIMade->loadBlockPose1(ss.str());

		ss.str("");
		ss << "SpaceInvader_Bomb_pose_2.txt";
		pShipIMade->loadBlockPose2(ss.str());

		ss.str("");
		ss << "SpaceInvader_Bomb_pose_3.txt";
		pShipIMade->loadBlockPose3(ss.str());


		// early exit
		return pShipIMade;
	}

	if (typeOfShip.command == "Space_Invader_Missile")
	{
		//		iShip* pShipIMade = new cBomberShip();
		//cSpaceInvader* pShipIMade = new cSpaceInvader();
		cSpaceInvader_Missile* pShipIMade = new cSpaceInvader_Missile();

		pShipIMade->myShipName = typeOfShip.vec_sData[0];
		pShipIMade->setPosition(typeOfShip.vec_fData[0]);

		cMesh* pModel = new cMesh();

		pModel->meshName = "Invader_Single_Cube.ply";

		pModel->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pModel->bUseWholeObjectDiffuseColour = true;
		pModel->bDontLight = true;
		pModel->textureNames[0] = "White.bmp";
		pModel->textureRatios[0] = 1.0f;

		// Add a bunch of child objects
		for (unsigned int index = 0; index != cSpaceInvader_Missile::MAX_NUMBER_OF_CHILD_BLOCKS; index++)
		{
			cMesh* pBlock = new cMesh();
			pBlock->meshName = "Invader_Single_Cube.ply";
			pBlock->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			pBlock->bUseWholeObjectDiffuseColour = true;
			pBlock->bDontLight = true;
			pBlock->textureNames[0] = "White.bmp";
			pBlock->textureRatios[0] = 1.0f;
			pBlock->bIsVisible = false;

			pModel->vec_pChildMeshes.push_back(pBlock);
		}

		pShipIMade->pTheMeshImUsing = pModel;

		// Tell the ship about the mediator
		pShipIMade->SetReciever(this->m_pTheMediator);

		std::stringstream ss;
		ss << "SpaceInvader_Missile_block.txt";
		pShipIMade->loadBlockPose1(ss.str());


		// early exit
		return pShipIMade;
	}


	// Unknown type of ship or other error
	this->m_lastError = "Unknown ship type";
	return NULL;
};


// Clears error when read
std::string cShipYard::getLastError(void)
{
	std::string error = this->m_lastError;
	this->m_lastError = "";
	return error;
}
