#include "globalThings.h"
//Stuff done in class with Michael Feeney

//cFlyCamera* g_pFlyCamera = NULL;

float moveStep = 100.0f;
float timeUntilMove = 1.0f;
float speedOfTime = 0.5f;
bool canWeMove = false;

iShip* g_pPlayer = NULL;
iShip* g_pUFO = NULL;
iShip* g_pMissile = NULL;

cVAOManager* g_pVAOManager = NULL;
cShaderManager* g_pShaderManager = NULL;
cBasicTextureManager* g_pTextureManager = NULL;
cShipYard* pShipYard = NULL;

cAnimationControl* g_pAnimationControl = NULL;

cLightManager* g_pTheLights = NULL;

cMediator* g_pTheMediator = NULL;

// List of objects to draw
std::vector< cMesh* > g_vec_pMeshes;
cEntityOrganizer* g_pEntityOrganizer;


bool nearbyWireFrame = false;

//glm::vec3 g_cameraEye = glm::vec3(0.0f, 0.0f, +4.0f);
//glm::vec3 g_cameraAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraEye = glm::vec3(0.0f, 2.0f, -4.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);


// This got moved into the light manager class
//extern cLightHelper* g_pTheLightHelper;


//std::map<unsigned int, std::vector<glm::vec3>> g_debugNormals;
bool g_turnDebugNormalsOn = false;


cMesh* g_pDebugSphere = NULL;
bool g_bShowDebugShere = true;

bool g_bShowDebugObjects = false;


unsigned int g_selectedObject = 0;
unsigned int g_selectedLight = 3;       //3rd light in the vector (blue spotlight over bed)

// Used for the texture height displacement example
glm::vec3 g_heightMapUVOffsetRotation = glm::vec3(0.0f, 0.0f, 0.0f);

// This will be printed in the title bar
std::string g_TitleText = "";

