#include "globalThings.h"
//Stuff done in class with Michael Feeney

bool g_StartUp(GLFWwindow* pWindow)
{

	//::g_pFlyCamera = new cFlyCamera();

	// Create the shader manager
	::g_pShaderManager = new cShaderManager();

	::g_pTheLights = new cLightManager();

	::g_pVAOManager = new cVAOManager();

	::g_pTextureManager = new cBasicTextureManager();

	::g_pTheMediator = new cMediator();
	//::g_pTheLights->SetReciever(::g_pTheMediator);

	//sMessage setLightManagerPointer;
	//setLightManagerPointer.command = "Set Light Manager Pointer";

	//void* pVoidPointerToTheLightManager = (void*)::g_pTheLights;
	//setLightManagerPointer.vec_pVoidPointers.push_back(pVoidPointerToTheLightManager);

	////	setLightManagerPointer.vec_pVoidPointers.push_back( (void*)::g_pTheLights );
	//::g_pTheMediator->RecieveMessage(setLightManagerPointer);

	::g_pEntityOrganizer = new cEntityOrganizer();

	pShipYard = new cShipYard();

	::g_pAnimationControl = new cAnimationControl();

	return true;
}
