#include "globalThings.h"
#include "cSpaceInvader_Missile.h"
//Stuff done in class with Michael Feeney

#include <sstream>
#include <iostream>


// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

//void LoadBlockFile(std::string InvaderName, std::string blockFileName)
//{
//    std::ifstream blockFile(blockFileName.c_str());
//
//    if (!blockFile.is_open())
//    {
//        return;
//    }
//
//    sMessage setBlocks;
//    setBlocks.command = "SetBlocks";
//
//    
//
//    iShip* sendCommandTo = ::g_pEntityOrganizer->find_pShipByName(InvaderName);
//    setBlocks.vec_fData.push_back(glm::vec4(100.0f, 100.0f, 100.0f, 100.0f));
//    sendCommandTo->RecieveMessage(setBlocks);
//
//    blockFile.close();
//
//    return;
//}


void LoadTheAnimationCommands(cAnimationControl* pAnimationControl);
void PermanentCommandGroup(cAnimationControl* pAnimationControl);
void WaypointCommand(cAnimationControl* pAnimationControl);

/*static*/ void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (!::canWeMove)
        {
            ::canWeMove = true;
            std::cout << "Start Moving!" << std::endl;
        }
    }//if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (::canWeMove)
        {
            if (!((cSpaceInvader_Missile*)::g_pMissile)->imFlying)
            {
                std::cout << "Pew!  But only if there's one!" << std::endl;
                sMessage launch;
                launch.command = "Launch";
                launch.vec_fData.push_back(glm::vec4(::g_pPlayer->getPosition().x + 70.0f, ::g_pPlayer->getPosition().y, ::g_pPlayer->getPosition().z, 1.0f));
                ::g_pMissile->RecieveMessage(launch);
            }
        }
    }//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)


    // From the help review session.
    // Move the camera to a location and look somewhere
    //if (key == GLFW_KEY_L && action == GLFW_PRESS)
    //{
    //    // Move the eye
    //    ::g_pFlyCamera->setEye(glm::vec3(8631.0f, -1487.0f, 13010.0f));

    //    // "look at" something.
    //    // Since we are using the fly camera, we can't just set the eye.
    //    //::g_pFlyCamera->setAt(glm::vec3(0.0f, 0.0f, 0.0f));

    //    ::g_pFlyCamera->Yaw_LeftRight(-80.0f);
    //    ::g_pFlyCamera->Pitch_UpDown(-10.0f);

    //}

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        LoadTheAnimationCommands(::g_pAnimationControl);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        PermanentCommandGroup(::g_pAnimationControl);
    }
    if (key == GLFW_KEY_3  && action == GLFW_PRESS)
    {
        WaypointCommand(::g_pAnimationControl);
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;


    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        // I have the farther meshes being drawn in wireframe, so this is being changed to a global bool
        //::nearbyWireFrame = !::nearbyWireFrame;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        // This triple for loop goes through and turns all the meshes to wireframe, however, to try and help with framerate issues,
        for (std::vector<cMesh*>::iterator it = ::g_vec_pMeshes.begin();
            it != ::g_vec_pMeshes.end(); it++)
        {
            (*it)->bIsWireframe = !(*it)->bIsWireframe;
            for (std::vector<cMesh*>::iterator iT = (*it)->vec_pChildMeshes.begin(); iT != (*it)->vec_pChildMeshes.end(); iT++)
            {
                (*iT)->bIsWireframe = !(*iT)->bIsWireframe;
                for (std::vector<cMesh*>::iterator It = (*iT)->vec_pChildMeshes.begin(); It != (*iT)->vec_pChildMeshes.end(); It++)
                {
                    (*It)->bIsWireframe = !(*It)->bIsWireframe;
                }
            }
        }
    }

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        ::g_bShowDebugObjects = !::g_bShowDebugObjects;
    }

    

    //    // Shift down?
    //    if ( mods == GLFW_MOD_SHIFT )       // 0x0001   0000 0001
    //    {
    //        // ONLY shift is down
    //    }
    //    // Control down?
    //    if ( mods == GLFW_MOD_CONTROL  )    // 0x0002   0000 0010
    //    // Alt down?
    //    if ( mods == GLFW_MOD_ALT   )       // 0x0004   0000 0100

        //   0000 0111 
        // & 0000 0001
        // -----------
        //   0000 0001 --> Same as the shift mask

        // Use bitwise mask to filter out just the shift
    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }


    //   // If JUST the shift is down, move the "selected" object
       if ( bShiftDown && (!bControlDown) && (!bAltDown) )
       {
           
    
    
       }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

       // If JUST the ALT is down, move the "selected" light
    if ((!bShiftDown) && (!bControlDown) && bAltDown)
    {
        

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
