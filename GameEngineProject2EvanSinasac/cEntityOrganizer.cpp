#include "cEntityOrganizer.h"

cEntityOrganizer::cEntityOrganizer()
{
}

cEntityOrganizer::~cEntityOrganizer()
{
}

void cEntityOrganizer::updateAllShips(double deltaTime)
{
    this->updateAllShips(this->m_Default_Ship_Group, deltaTime);
    return;
}

void cEntityOrganizer::updateAllShips(std::string groupName, double deltaTime)
{
    std::vector<iShip*>* p_vecShips = this->m_findCollectionOfShips(groupName);
    if ( p_vecShips )
    {
        for ( unsigned int index = 0; index != p_vecShips->size(); index++ )
        {
            (*p_vecShips)[index]->Update(deltaTime);
        }
    }//if ( p_vecShips )
    return;
}

// Don't keep this pointer around for long...
std::vector<iShip*>* cEntityOrganizer::get_pVecShipsForUpdate(std::string groupName)
{
    return this->m_findCollectionOfShips(groupName);
}

std::vector<iShip*>* cEntityOrganizer::get_pVecShipsForUpdate(void)
{
    return this->get_pVecShipsForUpdate(this->m_Default_Ship_Group);
}

// These return true if the call worked (did what was asked)
bool cEntityOrganizer::addMesh(std::string groupName, cMesh* pMesh, bool bAllowReplacement /*=false*/)
{
    // Find the group
    std::vector<cMesh*>* pVecMeshes = this->m_findCollectionOfMeshes(groupName);
    if ( ! pVecMeshes )
    {
        this->m_lastError = "Error: Didn't find that group of meshes: " + groupName;
        return false;
    }
    
    if ( ! bAllowReplacement )
    {
        // See if we already have this
        if ( this->m_findMesh(pMesh, pVecMeshes ) )
        {
            // Already there
            this->m_lastError = "Error: addMesh() tried to add a duplicate mesh, but isn't allowing replacement.";
            return false;
        }
    }

    // Add it
    pVecMeshes->push_back(pMesh);

    return true;
}

bool cEntityOrganizer::addMesh( cMesh* pMesh, bool bAllowReplacement /*=false*/)
{
    return this->addMesh( this->m_Default_Mesh_Group, pMesh, bAllowReplacement );
}

bool cEntityOrganizer::addShip(iShip* pShip, bool bAllowReplacement /*=false*/)
{
    return this->addShip( this->m_Default_Ship_Group, pShip, bAllowReplacement);
}

bool cEntityOrganizer::addShip(std::string groupName, iShip* pShip, bool bAllowReplacement /*=false*/)
{
    // Find the group
    std::vector<iShip*>* pVecShips = this->m_findCollectionOfShips(groupName);
    if ( !pVecShips )
    {
        this->m_lastError = "Error: Didn't find that group of ships: " + groupName;
        return false;
    }

    if ( !bAllowReplacement )
    {
        // See if we already have this
        if ( this->m_findShip(pShip, pVecShips) )
        {
            // Already there
            this->m_lastError = "Error: addShip() tried to add a duplicate ship, but isn't allowing replacement.";
            return false;
        }
    }

    // Add it
    pVecShips->push_back(pShip);

    return true;
}

bool cEntityOrganizer::m_findMesh(const cMesh* pMeshToFind, const std::vector<cMesh*>* p_vecMeshes)
{
    for ( unsigned int index = 0; index != p_vecMeshes->size(); index++ )
    {
        if ( (*p_vecMeshes)[index] == pMeshToFind )
        {
            // Found it
            return true;
        }
    }
    // Nope, didn't find it
    return false;
}
    

bool cEntityOrganizer::m_findShip(const iShip* pShipToFind, const std::vector<iShip*>* p_vecShips)
{
    for ( unsigned int index = 0; index != p_vecShips->size(); index++ )
    {
        if ( (*p_vecShips)[index] == pShipToFind )
        {
            // Found it
            return true;
        }
    }
    // Nope, didn't find it
    return false;
}

// Don't keep this pointer around for long...
std::vector<cMesh*>* cEntityOrganizer::get_pVecMeshesForDrawing(std::string groupName)
{
    return this->m_findCollectionOfMeshes(groupName);
}

std::vector<cMesh*>* cEntityOrganizer::get_pVecMeshesForDrawing(void)
{
    return this->m_findCollectionOfMeshes(this->m_Default_Mesh_Group);
}


std::vector<cMesh*>* cEntityOrganizer::m_findCollectionOfMeshes(std::string collectionName)
{
    std::map< std::string /*collection name*/, std::vector< cMesh* > >::iterator itGroup 
        = this->mapGroupOfMeshes.find(collectionName);
    // Found it?
    if ( itGroup != this->mapGroupOfMeshes.end() )
    {   
        // Yup
        std::vector<cMesh*>* pVecFound = &(itGroup->second);
        return pVecFound;
    }
    // Didn't find it
    return NULL;
}

std::vector<iShip*>* cEntityOrganizer::m_findCollectionOfShips(std::string collectionName)
{
    std::map< std::string /*collection name*/, std::vector< iShip* > >::iterator itGroup
        = this->mapGroupOfShips.find(collectionName);
    // Found it?
    if ( itGroup != this->mapGroupOfShips.end() )
    {
        // Yup
        std::vector<iShip*>* pVecFound = &(itGroup->second);
        return pVecFound;
    }
    // Didn't find it
    return NULL;
}

// Uses the default group name
cMesh* cEntityOrganizer::find_pMeshByFriendlyName(
    const std::string& friendlyNameToFind,
    bool bRecursiveSearch /*=true*/)
{
    return this->find_pMeshByFriendlyName( friendlyNameToFind, this->m_Default_Ship_Group, bRecursiveSearch );
}


cMesh* cEntityOrganizer::find_pMeshByFriendlyName(
    const std::string& friendlyNameToFind,
    const std::string& groupName,
    bool bRecursiveSearch /*=true*/)
{
    std::vector<cMesh*>* pVecMeshes = this->m_findCollectionOfMeshes(groupName);
    if ( ! pVecMeshes )
    {
        this->m_lastError = "Error: Didn't find that mesh group name: " + groupName;
        return NULL;
    }

    return this->m_find_pMeshByFriendlyName(friendlyNameToFind, pVecMeshes, bRecursiveSearch);
}

iShip* cEntityOrganizer::find_pShipByName( 
    const std::string& nameToFind )
{
    return this->find_pShipByName( nameToFind, this->m_Default_Ship_Group );
}

iShip* cEntityOrganizer::find_pShipByName(
    const std::string& nameToFind,
    const std::string& groupName )
{
    std::vector<iShip*>* pVecShips = this->m_findCollectionOfShips(groupName);
    if ( !pVecShips )
    {
        this->m_lastError = "Error: Didn't find that mesh group name: " + groupName;
        return NULL;
    }

    for ( unsigned int index = 0; index != pVecShips->size(); index++ )
    {
        iShip* pCurpShip = (*pVecShips)[index];

        if ( pCurpShip->getName() == nameToFind )
        {
            // Found it
            return pCurpShip;
        }
    }//for ( unsigned int index

    // Didn't find that ship
    return NULL;
}

cMesh* cEntityOrganizer::m_find_pMeshByFriendlyName(std::string friendlyNameToFind, std::vector<cMesh*>* p_vecMeshes, bool bDoRecursiveSearch /*=true*/ )
{
    for ( unsigned int index = 0; index != p_vecMeshes->size(); index++ )
    {
        cMesh* pCurMesh = (*p_vecMeshes)[index];

        if ( pCurMesh->friendlyName == friendlyNameToFind )
        {
            // Found it
            return pCurMesh;
        }

        if ( bDoRecursiveSearch )
        {
            cMesh* pChildMesh = this->m_find_pMeshByFriendlyName( friendlyNameToFind, &(pCurMesh->vec_pChildMeshes), bDoRecursiveSearch );
            if ( pChildMesh != NULL )
            {
                // Found it (it's a child)
                return pChildMesh;
            }
        }//if ( bDoRecursiveSearch )
    }//for ( unsigned int index

    // Nope. Didn't find it
    return NULL;
}

cMesh* cEntityOrganizer::m_find_pMeshByUniqueID(unsigned int uniqueID, std::vector<cMesh*>* p_vecMeshes, bool bDoRecursiveSearch /*=true*/ )
{
    for ( unsigned int index = 0; index != p_vecMeshes->size(); index++ )
    {
        cMesh* pCurMesh = (*p_vecMeshes)[index];

        if ( pCurMesh->getUniqueID() == uniqueID )
        {
            // Found it
            return pCurMesh;
        }

        if ( bDoRecursiveSearch )
        {
            cMesh* pChildMesh = this->m_find_pMeshByUniqueID(uniqueID, &(pCurMesh->vec_pChildMeshes), bDoRecursiveSearch);
            if ( pChildMesh != NULL )
            {
                // Found it (it's a child)
                return pChildMesh;
            }
        }//if ( bDoRecursiveSearch )
    }//for ( unsigned int index

    // Nope. Didn't find it
    return NULL;
}

void cEntityOrganizer::addMeshGroup(std::string groupName)
{
    // Already got this one?
    std::vector<cMesh*>* pVecMeshes = this->m_findCollectionOfMeshes(groupName);
    if ( ! pVecMeshes )
    {
        //Nope, so add it
        this->mapGroupOfMeshes[groupName] = std::vector<cMesh*>();
    }
    return;
}

void cEntityOrganizer::addShipGroup(std::string groupName)
{
    // Already got this one?
    std::vector<iShip*>* pVecShips = this->m_findCollectionOfShips(groupName);
    if ( !pVecShips )
    {
        //Nope, so add it
        this->mapGroupOfShips[groupName] = std::vector<iShip*>();
    }
    return;}


void cEntityOrganizer::setDefaultMeshGroupName(std::string groupName)
{
    this->m_Default_Mesh_Group = groupName;
    return;
}

std::string cEntityOrganizer::getDefaultMeshGroupName(void)
{
    return this->m_Default_Mesh_Group;
}

void cEntityOrganizer::setDefaultShipGroupName(std::string groupName)
{
    this->m_Default_Ship_Group = groupName;
    return;
}

std::string cEntityOrganizer::getDefaultShipGroupName(void)
{
    return this->m_Default_Ship_Group;
}
