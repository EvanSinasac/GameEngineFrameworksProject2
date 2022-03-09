#include "cAnimationControl.h"

// Here's the specific commands
#include "cCmd_Wait.h"
#include "cCmd_MoveToRelativeTime_1.h"
#include "cCmd_RotateToRelativeTime_1.h"
#include "cCmd_FollowWaypoints_InRelativeTime_1.h"
#include "cCmd_FollowObject_1.h"
#include "cLocation_Based_Trigger_Command_1.h"
#include "globalThings.h"

void LoadTheAnimationCommands(cAnimationControl* pAnimationControl)
{

	//    ____        _               _          _                 _   _             
	//   | __ )  __ _(_)___  ___     / \   _ __ (_)_ __ ___   __ _| |_(_) ___  _ __  
	//   |  _ \ / _` | / __|/ __|   / _ \ | '_ \| | '_ ` _ \ / _` | __| |/ _ \| '_ \ 
	//   | |_) | (_| | \__ \ (__   / ___ \| | | | | | | | | | (_| | |_| | (_) | | | |
	//   |____/ \__,_|_|___/\___| /_/   \_\_| |_|_|_| |_| |_|\__,_|\__|_|\___/|_| |_|
	//                                                                               
	cCommandGroup* pCutScene_1 = pAnimationControl->pCreateCommandGroup(1);

	//    cCommand_WaitFiveSeconds* pWait5Sec = new cCommand_WaitFiveSeconds();
	//iCommand* pCmdWait = new cCmd_Wait();
	//sMessage msgWait10Seconds;
	//msgWait10Seconds.vec_fData.push_back(glm::vec4(10.0f, 0.0f, 0.0f, 0.0f));
	//pCmdWait->Initialize(msgWait10Seconds);

	//// Add this 1 serial command to the group
	//pCutScene_1->AddCommandSerial(pCmdWait);

	//iCommand* pRotate = new cCmd_RotateEnginesOnSpikesShip();
	//pCutScene_1->AddCommandSerial(pRotate);

	iCommand* pCmdMove = new cCmd_MoveToRelativeTime_1();
	sMessage msgMoveTo;
	msgMoveTo.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, 50.0f, 0.0f), 10.0f));
	// ::g_pPlayer is set in main when I run createTheShips.  Could use the ::g_pEntityOrganizer to find it, search for the name of the ship
	// But I did this for ease of finding anyways
	((cCmd_MoveToRelativeTime_1*)pCmdMove)->setShip(::g_pPlayer);
	pCmdMove->Initialize(msgMoveTo);
	pCutScene_1->AddCommandSerial(pCmdMove);

	iCommand* pCmdRotate = new cCmd_RotateToRelativeTime_1();
	sMessage msgRotateTo;
	msgRotateTo.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, glm::radians(90.0f), 0.0f), 5.0f));
	((cCmd_RotateToRelativeTime_1*)pCmdRotate)->setShip(::g_pPlayer);
	pCmdRotate->Initialize(msgRotateTo);
	pCutScene_1->AddCommandParallel(pCmdRotate);

	iCommand* pCmdRotateBack = new cCmd_RotateToRelativeTime_1();
	sMessage msgRotateBack;
	msgRotateBack.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f));
	((cCmd_RotateToRelativeTime_1*)pCmdRotateBack)->setShip(::g_pPlayer);
	pCmdRotateBack->Initialize(msgRotateBack);
	pCutScene_1->AddCommandSerial(pCmdRotateBack);

	pAnimationControl->AddCommandGroup(pCutScene_1);


	cCommandGroup* pCutScene_2 = pAnimationControl->pCreateCommandGroup(2);

	// Command Follow Waypoints - first value in vec_fData is the starting waypoint and the time
	iCommand* pCmdFollowWaypoints = new cCmd_FollowWaypoints_InRelativeTime_1();
	sMessage msgFollowWaypoints;
	((cCmd_FollowWaypoints_InRelativeTime_1*)pCmdFollowWaypoints)->setShip(::g_pPlayer);
	// Set waypoints then initialize
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(20.0f, 0.0f, 0.0f), 10.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(20.0f, 20.0f, 0.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(-20.0f, 20.0f, 0.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(-20.0f, 0.0f, 0.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(10.0f, 0.0f, 0.0f), 1.0f));
	// This only works if we're running one command at a time.  This needs to be in start and start calls initialize for it to work in sequence
	// But then that causes the same issue where the values are hard coded and not configurable
	// Sooooooo, instead initialize will store values that get set at start.  (i.e. no more SetVelocity in initialize)
	pCmdFollowWaypoints->Initialize(msgFollowWaypoints);
	pCutScene_2->AddCommandSerial(pCmdFollowWaypoints);

	pAnimationControl->AddCommandGroup(pCutScene_2);


	// For the follow command, gunna use ::g_pEntityOrganizer to find the ship I want.
	// For project 2 it'll be the one I2 ship I made




	


	// Add a permanent command
	//cCommandGroup* pCG_SpaceDebris = pAnimationControl->pCreateCommandGroup();
	//iCommand* pCmdFlyingSpaceDebris = new cCmd_SpaceDebris();
	//pCG_SpaceDebris->AddCommandParallel(pCmdFlyingSpaceDebris);

	//pAnimationControl->AddPermanentCommandGroup(pCG_SpaceDebris);





	return;
}


void PermanentCommandGroup(cAnimationControl* pAnimationControl)
{
	cCommandGroup* permGroup = pAnimationControl->pCreateCommandGroup(3);

	iCommand* followCmd = new cCmd_FollowObject_1();
	sMessage followMsg;
	// Offset position, then max distance
	// This should make it try to stay 2 units above the target
	followMsg.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, 50.0f, 10.0f), 50.0f));
	followMsg.vec_fData.push_back(glm::vec4(20.0f, 0.0f, 0.0f, 1.0f));
	((cCmd_FollowObject_1*)followCmd)->setOurShip(::g_pEntityOrganizer->find_pShipByName("I2"));
	((cCmd_FollowObject_1*)followCmd)->setTargetShip(::g_pPlayer);
	followCmd->Initialize(followMsg);
	permGroup->AddCommandParallel(followCmd);

	pAnimationControl->AddPermanentCommandGroup(permGroup);

}

void TriggerCommand(cAnimationControl* pAnimationControl)
{
	cCommandGroup* triggerGroup = pAnimationControl->pCreateCommandGroup(4);

	iCommand* triggerCmd = new cLocation_Based_Trigger_Command_1();
	sMessage triggerMsg;
	// world location and distance from location
	triggerMsg.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 20.0f));
	((cLocation_Based_Trigger_Command_1*)triggerCmd)->setTriggerShip(::g_pPlayer);
	triggerCmd->Initialize(triggerMsg);
	triggerGroup->AddCommandParallel(triggerCmd);

	pAnimationControl->AddPermanentCommandGroup(triggerGroup);
}

void LocationTriggerCommand(cAnimationControl* pAnimationControl, iShip* spinnyShip)
{
	// Make a bunch of things if something gets close enough.
	// Idk, make the ship spin or something
	// But only if the ship isn't already spinning
	cCommandGroup* locationTriggerGroup = pAnimationControl->pCreateCommandGroup(spinnyShip->getShipTypeAsUInt());

	iCommand* locationTriggerCmd = new cCmd_RotateToRelativeTime_1();
	sMessage locationTriggerRotateMsg;
	locationTriggerRotateMsg.vec_fData.push_back(glm::vec4(glm::radians<float>(180.0f), 0.0f, glm::radians<float>(-180.0f), 1.5f));
	((cCmd_RotateToRelativeTime_1*)locationTriggerCmd)->setShip(spinnyShip);
	locationTriggerCmd->Initialize(locationTriggerRotateMsg);
	locationTriggerGroup->AddCommandSerial(locationTriggerCmd);

	iCommand* rotateBackCmd = new cCmd_RotateToRelativeTime_1();
	sMessage rotateBackMsg;
	rotateBackMsg.vec_fData.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.5f));
	((cCmd_RotateToRelativeTime_1*)rotateBackCmd)->setShip(spinnyShip);
	rotateBackCmd->Initialize(rotateBackMsg);
	locationTriggerGroup->AddCommandSerial(rotateBackCmd);

	pAnimationControl->AddCommandGroup(locationTriggerGroup);
	
}

void WaypointCommand(cAnimationControl* pAnimationControl)
{
	cCommandGroup* pCutScene_2 = pAnimationControl->pCreateCommandGroup(5);

	// Command Follow Waypoints - first value in vec_fData is the starting waypoint and the time
	iCommand* pCmdFollowWaypoints = new cCmd_FollowWaypoints_InRelativeTime_1();
	sMessage msgFollowWaypoints;
	((cCmd_FollowWaypoints_InRelativeTime_1*)pCmdFollowWaypoints)->setShip(::g_pEntityOrganizer->find_pShipByName("I1"));
	// Set waypoints then initialize
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(10.0f, 10.0f, 10.0f), 10.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(10.0f, 15.0f, 30.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(15.0f, -10.0f, 0.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f));
	msgFollowWaypoints.vec_fData.push_back(glm::vec4(glm::vec3(100.0f, 10.0f, 20.0f), 1.0f));

	pCmdFollowWaypoints->Initialize(msgFollowWaypoints);
	pCutScene_2->AddCommandSerial(pCmdFollowWaypoints);

	pAnimationControl->AddCommandGroup(pCutScene_2);
}
