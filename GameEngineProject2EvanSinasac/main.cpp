//Evan Sinasac - 1081418
//INFO6044 Game Engine (Project 2)
//main.cpp description:
//				The purpose of this project is to create a system to handle serial and parallel commands with scripts that 
//				can handle various behavious as described by the project requirements

#include "GLCommon.h"
#include "GLMCommon.h"
//Shader stuff in global things
#include "globalThings.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "cShipyard.h"
#include <chrono>
#include <thread>
//#include "cSpaceInvader_I1.h"


float lastX = 600.0f;
float lastY = 320.0f;
bool firstMouse = true;
float cameraYaw = 90.0f;
float cameraPitch = 0.0f;

double deltaTime = 0.0;

int numberOfTransparentObjects = 0;

double ufoTimer = 15.0f;
bool ufoAlive = false;

std::vector<std::string> modelLocations;


//Function signature for DrawObject()
void DrawObject(
	cMesh* pCurrentMesh,
	glm::mat4 matModel,
	GLint matModel_Location,
	GLint matModelInverseTranspose_Location,
	GLuint program,
	cVAOManager* pVAOManager);

//void LoadTheAnimationCommands(cAnimationControl* pAnimationControl);
void TriggerCommand(cAnimationControl* pAnimationControl);
void LocationTriggerCommand(cAnimationControl* pAnimationControl, iShip* spinnyShip);

//callbacks
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool loadWorldFile();		
bool loadLightsFile();
std::string g_grid[51][65];

void loadFinalModels();

void CreateTheShips(void);

int main(int argc, char** argv)
{
	GLFWwindow* pWindow;

	GLuint program = 0;		//0 means no shader program

	GLint mvp_location = -1;
	std::stringstream ss;
	float baseAtten = 0.75f;
	float randomAddedAtten;

	glfwSetErrorCallback(GLFW_error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	pWindow = glfwCreateWindow(1200, 640, "Graphics Project 2!", NULL, NULL);

	if (!pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(pWindow, GLFW_key_callback);

	glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
	glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
	glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
	glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
	glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);


	glfwSetCursorPosCallback(pWindow, mouse_callback);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(pWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	GLint max_uniform_location = 0;
	GLint* p_max_uniform_location = NULL;
	p_max_uniform_location = &max_uniform_location;
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

	std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

	// Create global things
	::g_StartUp(pWindow);

	cShaderManager::cShader vertShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\vertShader_01.glsl";
	vertShader.fileName = ss.str();

	cShaderManager::cShader geomShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\geomShader.glsl";
	geomShader.fileName = ss.str();;

	cShaderManager::cShader fragShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\fragShader_01.glsl";
	fragShader.fileName = ss.str();

	if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, geomShader, fragShader))
	{
		std::cout << "Shader compiled OK" << std::endl;
		// 
		// Set the "program" variable to the one the Shader Manager used...
		program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
	}
	else
	{
		std::cout << "Error making shader program: " << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
	}


	// Select the shader program we want to use
	// (Note we only have one shader program at this point)
	glUseProgram(program);

	// *******************************************************
	// Now, I'm going to "load up" all the uniform locations
	// (This was to show how a map could be used)
	cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");
	int theUniformIDLoc = -1;
	theUniformIDLoc = glGetUniformLocation(program, "matModel");

	pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;

	// Or...
	pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");

	pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
	pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

	pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"] = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");

	pShaderProc->mapUniformName_to_UniformLocation["eyeLocation"] = glGetUniformLocation(program, "eyeLocation");

	// *******************************************************

	// TODO: GO through all these and load them into the map instead, and then go through the errors and pull the location from the map instead
	//GLint mvp_location = -1;
	mvp_location = glGetUniformLocation(program, "MVP");

	// Get "uniform locations" (aka the registers these are in)
	GLint matModel_Location = glGetUniformLocation(program, "matModel");
	//	GLint matView_Location = glGetUniformLocation(program, "matView");
	GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
	GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

	GLint bDiscardTransparencyWindowsON_LocID = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");

	//Lights stuff here

//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light

	// Large scene light	CAN'T BE INDEX 0!!!!  Index 0 seems to overwrite all other lights, so it can be the spotlight or something, not sure why,
	// might be something in the shader

	// loads all the lights from a file instead of individually setting them here
	if (loadLightsFile())
	{
		std::cout << "loadLightsFile finished ok!" << std::endl;
	}
	else
	{
		std::cout << "loadLightsFile did NOT finish ok!  Aborting!" << std::endl;
		return -1;
	}

	// Turning light 0 as a spotlight over the origin
	::g_pTheLights->theLights[0].position = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
	::g_pTheLights->theLights[0].diffuse = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLights->theLights[0].specular = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	::g_pTheLights->theLights[0].atten = glm::vec4(0.001f, 0.0001f, 0.00001f, 1000.0f);
	::g_pTheLights->theLights[0].direction = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	::g_pTheLights->theLights[0].param1 = glm::vec4(1.0f, 20.0f, 30.0f, 1.0f);
	::g_pTheLights->theLights[0].param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	::g_pTheLights->TurnOffLight(0);


	::g_pEntityOrganizer->addMeshGroup("Space Invaders");
	::g_pEntityOrganizer->setDefaultMeshGroupName("Space Invaders");

	::g_pEntityOrganizer->addShipGroup("Space Invaders");
	::g_pEntityOrganizer->setDefaultShipGroupName("Space Invaders");

	/*if (loadTSVGrid())
	{
		std::cout << "loadTSVGrid finished ok!" << std::endl;
	}
	else
	{
		std::cout << "loadTSVGrid did NOT finish ok!  Aborting!" << std::endl;
		return -1;
	}*/

	// Get the uniform locations of the light shader values
	::g_pTheLights->SetUpUniformLocations(program);

	ss.str("");
	//ss << SOLUTION_DIR << "common\\assets\\models\\";
	ss << SOLUTION_DIR << "common\\assets\\models\\";

	::g_pVAOManager->setFilePath(ss.str());


	// Load ALL the models
	//loadMidtermModels();
	//loadProject2Models();
	loadFinalModels();


	unsigned int totalVerticesLoaded = 0;
	unsigned int totalTrianglesLoaded = 0;
	for (std::vector<std::string>::iterator itModel = modelLocations.begin(); itModel != modelLocations.end(); itModel++)
	{
		sModelDrawInfo theModel;
		std::string modelName = *itModel;
		std::cout << "Loading " << modelName << "...";
		if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
		{
			std::cout << "didn't work because: " << std::endl;
			std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
		}
		else
		{
			std::cout << "OK." << std::endl;
			std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
			totalTrianglesLoaded += theModel.numberOfTriangles;
			totalVerticesLoaded += theModel.numberOfVertices;
		}
	} //end of for (std::vector<std::string>::iterator itModel

	std::cout << "Done loading models." << std::endl;
	std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
	std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;

	//indexes 0-7 for opening and closing doors

	//World file stuff here
	if (loadWorldFile())
	{
		std::cout << "loadWorldFile finished OK" << std::endl;
	}
	else
	{
		std::cout << "loadWorldFile did not finish OK, aborting" << std::endl;
		return -1;
	}

	CreateTheShips();
	//LoadTheAnimationCommands(::g_pAnimationControl);
	TriggerCommand(::g_pAnimationControl);

	//Michael Feeney's BMP texture mapping
	//// Load the textures
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\";
	::g_pTextureManager->SetBasePath(ss.str());

	if (::g_pTextureManager->Create2DTextureFromBMPFile("White.bmp", true))
		//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	{
		std::cout << "Loaded the texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load the texture" << std::endl;
	}


	// Default don't have a texture
	if (::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true))
		//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	{
		std::cout << "Loaded the texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load the texture" << std::endl;
	}

	// Add a skybox texture
	std::string errorTextString;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\cubemaps\\";
	::g_pTextureManager->SetBasePath(ss.str());		// update base path to cube texture location

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
		"TropicalSunnyDayRight2048.bmp",	/* +X */	"TropicalSunnyDayLeft2048.bmp" /* -X */,
		"TropicalSunnyDayUp2048.bmp",		/* +Y */	"TropicalSunnyDayDown2048.bmp" /* -Y */,
		"TropicalSunnyDayFront2048.bmp",	/* +Z */	"TropicalSunnyDayBack2048.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}


	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
		"SpaceBox_right1_posX.bmp",		/* +X */	"SpaceBox_left2_negX.bmp" /* -X */,
		"SpaceBox_top3_posY.bmp",		/* +Y */	"SpaceBox_bottom4_negY.bmp" /* -Y */,
		"SpaceBox_front5_posZ.bmp",		/* +Z */	"SpaceBox_back6_negZ.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("City01",	
		"city_lf.bmp",		/* +X */	"city_rt.bmp" /* -X */,
		"city_dn.bmp",		/* +Y */	"city_up.bmp" /* -Y */,
		"city_ft.bmp",		/* +Z */	"city_bk.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	//// Turns out to make a "cube" map, the images need to be squares
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Skyrim",
		"Skyrim_Right.bmp",		/* +X */	"Skyrim_Left.bmp" /* -X */,
		"Skyrim_Top.bmp",		/* +Y */	"Skyrim_Bottom.bmp" /* -Y */,
		"Skyrim_Forward.bmp",		/* +Z */	"Skyrim_Back.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	// Make sphere for "skybox" before world file
	cMesh* sphereSky = new cMesh();
	//sphereSky->meshName = "Isosphere_Smooth_Normals.ply";
	// We are using a sphere with INWARD facing normals, so we see the "back" of the sphere
	sphereSky->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
	// 2 main ways we can do a skybox:
	//
	// - Make a sphere really big, so everything fits inside
	// (be careful of the far clipping plane)
	//
	// - Typical way is:
	//	- Turn off the depth test
	//	- Turn off the depth function (i.e. writing to the depth buffer)
	//	- Draw the skybox object (which can be really small, since it's not interacting with the depth buffer)
	//	- Once drawn:
	//		- Turn on the depth function
	//		- Turn on the depth test

	sphereSky->positionXYZ = ::cameraEye;	//glm::vec3(100.0f, -250.0f, 100.0f);
	//sphereSky->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	sphereSky->scale = glm::vec3(10000.0f);
	//sphereSky->bUseWholeObjectDiffuseColour = false;
	//sphereSky->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphereSky->bDontLight = true;
	sphereSky->clearTextureRatiosToZero();
	//sphereSky->textureNames[1] = "2k_jupiter.bmp";
	sphereSky->textureRatios[1] = 1.0f;
	//Draw it separately so don't add it to the vector
	//::g_vec_pMeshes.push_back(sphereSky);


	// My camera reset stuff	(might be broken, idk anymore)
	if (::cameraEye.x > 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = 180.f + (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x > 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = 90.f - (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x == 0.f)
	{
		if (::cameraEye.z >= 0.f)
		{
			::cameraYaw = 270.f;
		}
		else
		{
			::cameraYaw = 90.f;
		}
	}
	else if (::cameraEye.z == 0.f)
	{
		if (::cameraEye.x <= 0)
		{
			::cameraYaw = 0.f;
		}
		else
		{
			::cameraYaw = 180.f;
		}
	}
	//anyways, after figuring out the yaw, we set the target at the negative of the xz of the ::camera position and y=0 (this faces the ::camera towards the origin)
	::cameraTarget = glm::vec3(-1.f * ::cameraEye.x, 0, -1.f * ::cameraEye.z);
	glm::normalize(::cameraTarget);

	const double MAX_DELTA_TIME = 0.1;	//100 ms
	double previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(pWindow))
	{
		float ratio;
		int width, height;
		glm::mat4 matModel;				// used to be "m"; Sometimes it's called "world"
		glm::mat4 matProjection;        // used to be "p";
		glm::mat4 matView;              // used to be "v";

		double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
		previousTime = currentTime;

		glfwGetFramebufferSize(pWindow, &width, &height);
		ratio = width / (float)height;

		//Turn on the depth buffer
		glEnable(GL_DEPTH);         // Turns on the depth buffer
		glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// *******************************************************
		// Screen is cleared and we are ready to draw the scene...
		// *******************************************************

		// Update the title text
		glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());

		// Before sending the light info from the shader, we have to get those torches flickering
		for (unsigned int index = 1; index < 52; index++)
		{
			srand(time(NULL));
			randomAddedAtten = (rand() % 100) /100.0f;
			::g_pTheLights->theLights[index].atten.y = baseAtten * gGetRandBetween<float>(1.0f, 2.0f);
		}

		// Copy the light information into the shader to draw the scene
		::g_pTheLights->CopyLightInfoToShader();

		matProjection = glm::perspective(1.0f,	// FOV variable later
			ratio,
			0.1f,								// Near plane
			1'000'000.0f);						// Far plane

		matView = glm::mat4(1.0f);
		matView = glm::lookAt(::cameraEye,
			::cameraEye + ::cameraTarget,
			upVector);

		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
			1, GL_FALSE, glm::value_ptr(matView));


		glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));

		glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["eyeLocation"], ::cameraEye.x, ::cameraEye.y, ::cameraEye.z, 1.0f);

		// ***************************************
		// Geometry shader generate normals
		//  uniform bool bDrawNormals;
		//  uniform float debugNormalLength;

		GLint bDrawDebugNormals_LocID = glGetUniformLocation(program, "bDrawDebugNormals");
		if (::g_bShowDebugObjects)
		{
			glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_TRUE);

			GLint debugNormalLength_LocID = glGetUniformLocation(program, "debugNormalLength");
			glUniform1f(debugNormalLength_LocID, 0.1f);
		}
		else
		{
			glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_FALSE);
		}
		// ***************************************
		// Since this is a space game (most of the screen is filled with "sky"),
		// I'll draw the skybox first

		GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
		glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);

		// Move the "skybox object" with the camera
		sphereSky->positionXYZ = ::cameraEye;
		DrawObject(sphereSky, glm::mat4(1.0f),
			matModel_Location, matModelInverseTranspose_Location,
			program, ::g_pVAOManager);

		glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);

		//For transparency for the beakers, they need to be drawn farthest from the camera first, so we need to sort the last 4 meshes
		// Alpha transparency sorting
		//for (unsigned int index = ::g_vec_pMeshes.size() - numberOfTransparentObjects; index != g_vec_pMeshes.size() - 1; index++)
		//{
		//	//Distance is the difference in length between camera eye and mesh position
		//	float dist1, dist2;
		//	dist1 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index]->positionXYZ));
		//	dist2 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index + 1]->positionXYZ));
		//	//if the second object is farther, move it up
		//	if (dist2 > dist1)
		//	{
		//		cMesh* temp = ::g_vec_pMeshes[index + 1];
		//		::g_vec_pMeshes[index + 1] = ::g_vec_pMeshes[index];
		//		::g_vec_pMeshes[index] = temp;
		//	}

		//}

		// Call the animation control
		::g_pAnimationControl->Update(deltaTime);

//    ____  _     _         _   _           _       _         
//   / ___|| |__ (_)_ __   | | | |_ __   __| | __ _| |_ ___ _ 
//   \___ \| '_ \| | '_ \  | | | | '_ \ / _` |/ _` | __/ _ (_)
//    ___) | | | | | |_) | | |_| | |_) | (_| | (_| | ||  __/_ 
//   |____/|_| |_|_| .__/   \___/| .__/ \__,_|\__,_|\__\___(_)
//                 |_|           |_|                          

		if (::canWeMove)
		{
			::timeUntilMove -= deltaTime * ::speedOfTime;
			std::cout << ::timeUntilMove << std::endl;
			if (::timeUntilMove <= 0.0f)
			{
				::timeUntilMove = 1.0f;
				std::vector<iShip*>* p_MoveTheShips = ::g_pEntityOrganizer->get_pVecShipsForUpdate();
				if (p_MoveTheShips)
				{
					for (unsigned int index = 0; index != p_MoveTheShips->size(); index++)
					{
						if ((*p_MoveTheShips)[index]->getName() == "I1"
							|| (*p_MoveTheShips)[index]->getName() == "I2"
							|| (*p_MoveTheShips)[index]->getName() == "I3")
						{
							//std::this_thread::sleep_for(std::chrono::nanoseconds(5000)); //just makes everything pause for a sec
							sMessage timeToMove;
							timeToMove.command = "TimeToMove";
							(*p_MoveTheShips)[index]->RecieveMessage(timeToMove);
						}
					}
					// Once all the aliens have been moved, check their positions, if one is close enough to the edge of the "screen", they need to turn around
					for (unsigned int index = 0; index != p_MoveTheShips->size(); index++)
					{
						if ((*p_MoveTheShips)[index]->getName() == "I1"
							|| (*p_MoveTheShips)[index]->getName() == "I2"
							|| (*p_MoveTheShips)[index]->getName() == "I3")
						{
							if ((*p_MoveTheShips)[index]->getPosition().x >= 1500.0f
								|| (*p_MoveTheShips)[index]->getPosition().x <= -2000.0f)
							{
								::moveStep *= -1.0f;
								sMessage moveDown;
								moveDown.command = "MoveDown";
								for (unsigned int index = 0; index != p_MoveTheShips->size(); index++)
								{
									if ((*p_MoveTheShips)[index]->getName() == "I1"
										|| (*p_MoveTheShips)[index]->getName() == "I2"
										|| (*p_MoveTheShips)[index]->getName() == "I3")
									{
										(*p_MoveTheShips)[index]->RecieveMessage(moveDown);
									}
								}
								break;
							}
							if ((*p_MoveTheShips)[index]->getPosition().y <= 500.0f)
							{
								// Idk, do something to end the game
								//std::cout << "Game Over!" << std::endl;
								//::canWeMove = false;
							}
						}
					}
				}
			}
		} //if ::weCanMove


		//UFO logic
		//if (::canWeMove && !ufoAlive)
		//{
		//	ufoTimer -= deltaTime;
		//	if (ufoTimer <= 0.0)
		//	{
		//		ufoAlive = true;
		//		sMessage ufoAlive;
		//		ufoAlive.command = "Attack";
		//		ufoAlive.vec_fData.push_back(glm::vec4(1500.0f, 2000.0f, 450.0f, 1.0f));
		//		::g_pUFO->RecieveMessage(ufoAlive);
		//	}
		//}
		
		//if (::canWeMove && ufoAlive)
		//{
		//	::g_pUFO->setPosition(glm::vec3(::g_pUFO->getPosition().x - 200.0f * deltaTime, ::g_pUFO->getPosition().y, ::g_pUFO->getPosition().z));
		//	if (::g_pUFO->getPosition().x <= -2000.0f)
		//	{
		//		srand(time(NULL));
		//		ufoTimer = (rand() % 20) + 10;
		//		ufoAlive = false;
		//		sMessage ufoHide;
		//		ufoHide.command = "Hide";
		//		::g_pUFO->RecieveMessage(ufoHide);
		//	}
		//}

		
		// Get a pointer to the vector of ships JUST for this update
		std::vector<iShip*>* p_vecShips = ::g_pEntityOrganizer->get_pVecShipsForUpdate();
		if (p_vecShips)
		{
			int thisPass = p_vecShips->size();	// HACK, I was getting read access violation, this seems to have fixed it, but not always, idk.  It's midnight
			for (unsigned int index = 0; index != thisPass; index++)
			{
				iShip* pCurrentShip = (*p_vecShips)[index];

				// Do the physical movement integration update
				// TODO: Update the rotational position also
				sPhysicalProps physProps = pCurrentShip->getPhysicalProperties();

				// Updating the Cartesian position
				glm::vec3 deltaVel = physProps.acceleration * (float)deltaTime;
				physProps.velocity += deltaVel;

				glm::vec3 deltaPos = physProps.velocity * (float)deltaTime;
				physProps.position += deltaPos;

				pCurrentShip->setVelocity(physProps.velocity);
				pCurrentShip->setPosition(physProps.position);

				//physProps.orientationEuler.x += 0.01f;
				//physProps.orientationEuler.y += 0.003f;
				//physProps.orientationEuler.z -= 0.014;
				//pCurrentShip->setOrientationEuler(physProps.orientationEuler);


				// Update the objects (it's "brain" tick of the clock)
				pCurrentShip->Update(deltaTime);

				// Execute a command or command group if anything is close enough
				if (glm::distance(glm::vec3(-100.0f, 10.0f, 20.0f), pCurrentShip->getPosition()) <= 5.0f)
				{
					LocationTriggerCommand(::g_pAnimationControl, pCurrentShip);
				}

			}//for (unsigned int index
		}//if ( p_vecShips )
		

		// **********************************************************************
		// Draw the "scene" of all objects.
		// i.e. go through the vector and draw each one...
		// **********************************************************************
		for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
		{
			// So the code is a little easier...
			cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

			matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
			//mat4x4_identity(m);

			if (pCurrentMesh->bHasDiscardTexture)
			{
				// Discard texture
				{
					//GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
					GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->discardTexture);

					// I'm picking texture unit 30 since it's not in use.
					GLuint textureUnit = 30;
					glActiveTexture(textureUnit + GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, discardTextureNumber);
					GLint discardTexture_LocID = glGetUniformLocation(program, "discardTexture");
					glUniform1i(discardTexture_LocID, textureUnit);


					// Turn it on
					glUniform1f(bDiscardTransparencyWindowsON_LocID, (GLfloat)GL_TRUE);
					DrawObject(pCurrentMesh,
						matModel,
						matModel_Location,
						matModelInverseTranspose_Location,
						program,
						::g_pVAOManager);

					// Turn it off
					glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"], (GLfloat)GL_FALSE);
				}
			}
			else 
			{
				DrawObject(pCurrentMesh,
					matModel,
					matModel_Location,
					matModelInverseTranspose_Location,
					program,
					::g_pVAOManager);
			}
			
		}//for (unsigned int index

		std::vector<iShip*>* p_vecShipsToDraw = ::g_pEntityOrganizer->get_pVecShipsForUpdate();
		for (unsigned int index = 0; index != p_vecShipsToDraw->size(); index++)
		{

			// Note that I'm casting this as a iMeshDrawInfo object
			// so I can only see those methods:
			iShip* pShip = (*p_vecShipsToDraw)[index];

			cMesh* pShipsDrawingMesh = pShip->getMeshPointer();

			glm::mat4 matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
			//mat4x4_identity(m);

			DrawObject(pShipsDrawingMesh,
				matModel,
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);


		}//for (unsigned int index 


 		std::vector<cMesh*>* p_vecMeshesToDraw = ::g_pEntityOrganizer->get_pVecMeshesForDrawing();
		for (unsigned int index = 0; index != p_vecMeshesToDraw->size(); index++)
		{
			// So the code is a little easier...
			cMesh* pCurrentMesh = (*p_vecMeshesToDraw)[index];

			matModel = glm::mat4(1.0f);

			DrawObject(pCurrentMesh,
				matModel,
				matModel_Location,
				matModelInverseTranspose_Location,
				program,
				::g_pVAOManager);
		}


		// Scene is drawn
		// **********************************************************************

		// "Present" what we've drawn.
		glfwSwapBuffers(pWindow);        // Show what we've drawn

		// Process any events that have happened
		glfwPollEvents();

		// Handle OUR keyboard, mouse stuff
		handleAsyncKeyboard(pWindow, deltaTime);
		handleAsyncMouse(pWindow, deltaTime);

	} //end of while (!glfwWindowShouldClose(window))

	// All done, so delete things...
	::g_ShutDown(pWindow);


	glfwDestroyWindow(pWindow);

	glfwTerminate();
	exit(EXIT_SUCCESS);

} //end of main


// Modified from previous versions to take texture information
bool loadWorldFile()
{
	std::stringstream ss;
	std::stringstream sFile;


	ss << SOLUTION_DIR << "common\\assets\\worldFile.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open worldFile.txt");
		return false;
	}

	std::string nextToken;
	ss.str("");

	//Throwaway text describing the format of the file
	theFile >> nextToken;       //ModelFileName(extension)
	theFile >> nextToken;       //Position(x,y,z)
	theFile >> nextToken;       //Orientation(x,y,z)
	theFile >> nextToken;       //Scale(x,y,z)
	theFile >> nextToken;       //Colour(r,g,b,a)
	theFile >> nextToken;		//Light
	theFile >> nextToken;		//TextureOp
	theFile >> nextToken;		//TextureName1
	theFile >> nextToken;		//Ratio1
	theFile >> nextToken;		//TextureName2
	theFile >> nextToken;		//Ratio2
	theFile >> nextToken;		//TextureName3
	theFile >> nextToken;		//Ratio3
	theFile >> nextToken;		//DiscardTexture

	theFile >> nextToken;		//Camera(x,y,z)
	theFile >> nextToken;		//x position
	::cameraEye.x = std::stof(nextToken);
	theFile >> nextToken;		//y position
	::cameraEye.y = std::stof(nextToken);
	theFile >> nextToken;		//z position
	::cameraEye.z = std::stof(nextToken);

	theFile >> nextToken;		//number of transparent objects
	numberOfTransparentObjects = std::stoi(nextToken);

	//From here modify based on worldFile format
	while (theFile >> nextToken)    //this should always be the name of the model to load or end.  Potential error check, add a check for "ply" in the mdoel name
	{
		cMesh* curMesh = new cMesh;
		if (nextToken == "end")
		{
			break;
		}
		std::cout << nextToken << std::endl;        //Printing model names to console, just making sure we're loading ok.  Can be commented out whenever
		//First is the file name of model
		//ss << SOLUTION_DIR << "common\\assets\\models\\" << nextToken;		// don't need this with the setBasePath function for the loading intot the VAO
		curMesh->meshName = nextToken;
		//Next 3 are the position of the model
		theFile >> nextToken;                                               //x position for the model
		curMesh->positionXYZ.x = std::stof(nextToken);
		theFile >> nextToken;                                               //y position for the model
		curMesh->positionXYZ.y = std::stof(nextToken);
		theFile >> nextToken;                                               //z position for the model
		curMesh->positionXYZ.z = std::stof(nextToken);
		//Next 3 are the orientation of the model
		theFile >> nextToken;                                               //x orientation value
		//curMesh.orientationXYZ.x = std::stof(nextToken);
		curMesh->orientationXYZ.x = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //y orientation value
		//curMesh.orientationXYZ.y = std::stof(nextToken);
		curMesh->orientationXYZ.y = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //z orientation value
		//curMesh.orientationXYZ.z = std::stof(nextToken);
		curMesh->orientationXYZ.z = glm::radians(std::stof(nextToken));
		//Next is the scale to multiply the model by
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.x = std::stof(nextToken);
		theFile >> nextToken;
		curMesh->scale.y = std::stof(nextToken);
		theFile >> nextToken;
		curMesh->scale.z = std::stof(nextToken);
		//Next 3 are the r, g, b values for the model
		curMesh->bUseWholeObjectDiffuseColour = false;
		theFile >> nextToken;													//RGB red value
		curMesh->wholeObjectDiffuseRGBA.r = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB green value
		curMesh->wholeObjectDiffuseRGBA.g = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB blue value
		curMesh->wholeObjectDiffuseRGBA.b = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;
		curMesh->wholeObjectDiffuseRGBA.a = std::stof(nextToken);
		curMesh->alphaTransparency = std::stof(nextToken);

		// Light
		theFile >> nextToken;
		curMesh->bDontLight = (nextToken == "0");		// set to true or "0" in the worldFile to see textures without lighting

		// Texture operator
		theFile >> nextToken;
		curMesh->textureOperator = std::stoi(nextToken);

		// texture for mesh is in worldFile.txt now
		curMesh->clearTextureRatiosToZero();

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[1] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[1] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[2] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[2] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[3] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[3] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		if (curMesh->textureRatios[1] == 0.0f && curMesh->textureRatios[2] == 0.0f && curMesh->textureRatios[3] == 0.0f)
		{
			curMesh->textureRatios[0] = 1.0f;
		}

		// Discard texture
		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->discardTexture = nextToken;
			curMesh->bHasDiscardTexture = true;
		}

		curMesh->bUseWholeObjectDiffuseColour = true;

		::g_vec_pMeshes.push_back(curMesh);     //push the model onto our vector of meshes
		ss.str("");                         //reset the stringstream
	} //end of while
	theFile.close();
	return true;
}	//end of load world file

bool loadLightsFile()
{
	std::stringstream ss;
	std::stringstream sFile;


	ss << SOLUTION_DIR << "common\\assets\\lights.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open lights.txr");
		return false;
	}

	std::string nextToken;
	ss.str("");

	// Throw away description 
	theFile >> nextToken;		// position(x,y,z)
	theFile >> nextToken;		// diffuse(r,g,b)
	theFile >> nextToken;		// specular(r,g,b)
	theFile >> nextToken;		// atten(x,y,z,w)
	theFile >> nextToken;		// direction(x,y,z)
	theFile >> nextToken;		// param1(x,y,z)
	theFile >> nextToken;		// param2(x)

	::g_pTheLights->TurnOffLight(0);

	unsigned int index = 1;	// can't start at 0 because for some reason the 0 light over writes all other lights

	while (theFile >> nextToken)
	{
		if (nextToken == "end" || index >= cLightManager::NUMBER_OF_LIGHTS)
		{
			break;
		}
		glm::vec3 position;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec4 atten;
		glm::vec3 direction;
		glm::vec3 param1;
		float param2;

		// Position
		position.x = std::stof(nextToken);
		theFile >> nextToken;
		position.y = std::stof(nextToken);
		theFile >> nextToken;
		position.z = std::stof(nextToken);

		// Diffuse
		theFile >> nextToken;
		diffuse.x = std::stof(nextToken) / 255.0f;
		theFile >> nextToken;
		diffuse.y = std::stof(nextToken) / 255.0f;
		theFile >> nextToken;
		diffuse.z = std::stof(nextToken) / 255.0f;

		// Specular
		theFile >> nextToken;
		specular.x = std::stof(nextToken) / 255.0f;
		theFile >> nextToken;
		specular.y = std::stof(nextToken) / 255.0f;
		theFile >> nextToken;
		specular.z = std::stof(nextToken) / 255.0f;

		// Atten
		theFile >> nextToken;
		atten.x = std::stof(nextToken);
		theFile >> nextToken;
		atten.y = std::stof(nextToken);
		theFile >> nextToken;
		atten.z = std::stof(nextToken);
		theFile >> nextToken;
		atten.w = std::stof(nextToken);

		// Direction
		theFile >> nextToken;
		direction.x = std::stof(nextToken);
		theFile >> nextToken;
		direction.y = std::stof(nextToken);
		theFile >> nextToken;
		direction.z = std::stof(nextToken);
		direction = glm::normalize(direction);

		// Param1
		theFile >> nextToken;
		param1.x = std::stof(nextToken);;
		theFile >> nextToken;
		param1.y = std::stof(nextToken);
		theFile >> nextToken;
		param1.z = std::stof(nextToken);

		// Param2
		theFile >> nextToken;
		param2 = std::stof(nextToken);


		// Load everything into the lights
		::g_pTheLights->theLights[index].position = glm::vec4(position, 1.0f);
		::g_pTheLights->theLights[index].diffuse = glm::vec4(diffuse, 1.0f);
		::g_pTheLights->theLights[index].specular = glm::vec4(specular, 1.0f);
		::g_pTheLights->theLights[index].atten = glm::vec4(atten);
		::g_pTheLights->theLights[index].direction = glm::vec4(direction, 1.0f);
		::g_pTheLights->theLights[index].param1 = glm::vec4(param1, 1.0f);
		::g_pTheLights->theLights[index].param2 = glm::vec4(param2, 0.0f, 0.0f, 1.0f);

		index++;

	} //end of while

	theFile.close();
	return true;

} //end of load lights


//Figured out the math for how to do this from https://learnopengl.com/Getting-started/Camera and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
//Using the mouse position we calculate the direction that the camera will be facing
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if it's the start of the program this smooths out a potentially glitchy jump
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//find the offset of where the mouse positions have moved
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	//multiply by sensitivity so that it's not potentially crazy fast
	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	::cameraYaw += xOffset;         // The yaw is the rotation around the ::camera's y-axis (so we want to add the xOffset to it)
	::cameraPitch += yOffset;       // The pitch is the rotation around the ::camera's x-axis (so we want to add the yOffset to it)
	//This limits the pitch so that we can't just spin the ::camera under/over itself
	if (::cameraPitch > 89.0f)
		::cameraPitch = 89.0f;
	if (::cameraPitch < -89.0f)
		::cameraPitch = -89.0f;
	//calculations for the new direction based on the mouse movements
	glm::vec3 direction;
	direction.x = cos(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	direction.y = sin(glm::radians(::cameraPitch));
	direction.z = sin(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	::cameraTarget = glm::normalize(direction);
} //fly ::camera


void loadFinalModels()
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	// Models for the final project
	modelLocations.push_back("Invader_Single_Cube.ply");
}

void CreateTheShips()
{
	//cShipYard* pShipYard = new cShipYard();

	pShipYard->SetMediator(::g_pTheMediator);

	{ // Space Invaders
		float initialX = -1100.0f;
		float initialY = 2000.0f;
		float xOffset = 200.0f;
		float zOffset = 0.0f;
		float yOffset = 200.0f;

		int uniqueID;

		sMessage setBlocks;
		setBlocks.command = "SetBlocks";
		setBlocks.vec_fData.push_back(glm::vec4(100.0f, 100.0f, 100.0f, 100.0f));


		//iShip* sendCommandTo = ::g_pEntityOrganizer->find_pShipByName(InvaderName);
		

		// There would be a line of aliens of each type for the static screen
		sMessage invader;
		// Stuff for final, just building off of it for the animation controller stuff for project 2
		invader.command = "Space_Invader_I2";
		invader.vec_sData.push_back("I2");			// For project 2, this is the ship I'm going to look for to follow
		invader.vec_fData.push_back(glm::vec4(initialX, initialY, 500.0f, 1.0f));

		// For Game Engine Project 2
		invader.vec_fData[0] = glm::vec4(-100.0f, 10.0f, 20.0f, 1.0f);
		iShip* inv2 = pShipYard->makeAShip(invader);
		inv2->RecieveMessage(setBlocks);
		::g_pEntityOrganizer->addShip(inv2);

		invader.command = "Space_Invader_I1";
		invader.vec_sData[0] = "I1";
		invader.vec_fData[0] = glm::vec4(100.0f, 10.0f, 20.0f, 1.0f);
		iShip* inv3 = pShipYard->makeAShip(invader);
		inv3->RecieveMessage(setBlocks);
		::g_pEntityOrganizer->addShip(inv3);

		//for (unsigned int index = 0; index < 11; index++)
		//{
		//	invader.vec_fData[0] = (glm::vec4(initialX + (index * xOffset), initialY, 450.0f, 1.0f));
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}

		//invader.command = "Space_Invader_I1";
		//invader.vec_sData[0] = "I1";
		//for (unsigned int index = 0; index < 11; index++)
		//{
		//	invader.vec_fData[0] = glm::vec4(initialX + (index * xOffset), initialY - yOffset, 450.0f, 1.0f);
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}
		//for (unsigned int index = 0; index < 11; index++)
		//{
		//	invader.vec_fData[0] = glm::vec4(initialX + (index * xOffset), initialY - (yOffset * 2), 450.0f, 1.0f);
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}
		//

		//invader.command = "Space_Invader_I3";
		//invader.vec_sData[0] = "I3";
		//for (unsigned int index = 0; index < 11; index++)
		//{
		//	invader.vec_fData[0] = glm::vec4(initialX + (index * xOffset), initialY - (yOffset * 3), 450.0f, 1.0f);
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}
		//for (unsigned int index = 0; index < 11; index++)
		//{
		//	invader.vec_fData[0] = glm::vec4(initialX + (index * xOffset), initialY - (yOffset * 4), 450.0f, 1.0f);
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}
		//
		//// 4 shields on static screen
		//invader.command = "Space_Invader_Shield";
		//invader.vec_sData[0] = "Shield";
		//for (unsigned int index = 0; index < 4; index++)
		//{
		//	invader.vec_fData[0] = glm::vec4(initialX + (300.0f * (index + 1)) + (index * xOffset), initialY - (yOffset * 8), 450.0f, 1.0f);
		//	iShip* temp = pShipYard->makeAShip(invader);
		//	temp->RecieveMessage(setBlocks);
		//	::g_pEntityOrganizer->addShip(temp);
		//	//::g_vec_pMeshes.push_back(temp->getMeshPointer());
		//}

		//// No UFOs on the static screen, but we should make one
		//invader.command = "Space_Invader_UFO";
		//invader.vec_sData[0] = "UFO";
		//invader.vec_fData[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//::g_pUFO = pShipYard->makeAShip(invader);
		//::g_pUFO->RecieveMessage(setBlocks);
		sMessage hide;
		hide.command = "Hide";
		//::g_pUFO->RecieveMessage(hide);
		//::g_pEntityOrganizer->addShip(::g_pUFO);

		// Missile
		invader.command = "Space_Invader_Missile";
		invader.vec_sData[0] = "Missile";
		invader.vec_fData[0] = glm::vec4(0.0f, -1000.0f, 0.0f, 1.0f);
		::g_pMissile = pShipYard->makeAShip(invader);
		::g_pMissile->RecieveMessage(setBlocks);
		::g_pMissile->RecieveMessage(hide);
		::g_pEntityOrganizer->addShip(::g_pMissile);

		// And I'm going to just have the one "base" for the player to control (later)
		invader.command = "Space_Invader_Base";
		invader.vec_sData[0] = "Base";
		invader.vec_fData[0] = glm::vec4(0.0f, 0.0f, 450.0f, 1.0f);
		::g_pPlayer = pShipYard->makeAShip(invader);
		if (!g_pPlayer)
		{
			std::cout << pShipYard->getLastError() << std::endl;
		}
		::g_pPlayer->RecieveMessage(setBlocks);
		::g_pEntityOrganizer->addShip(::g_pPlayer);
		//::g_vec_pMeshes.push_back(::g_pPlayer->getMeshPointer());


	}
	return;
}

