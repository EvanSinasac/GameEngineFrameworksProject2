#pragma once

#include "cMesh.h"
#include "iShip.h"
#include <map>

// This class handles the following:
// * Holds the various lists of objects that might be drawn
// * Has the "find object by..." methods
// * Allows queries like what mesh models and textures are in use (according to the mesh information)
//
// This is mainly to encapsulate the g_vec_pMeshes.
// 
class cEntityOrganizer
{
public:
    cEntityOrganizer();
	~cEntityOrganizer();

    void addMeshGroup(std::string groupName);
    void addShipGroup(std::string groupName);

    void setDefaultMeshGroupName(std::string groupName);
    std::string getDefaultMeshGroupName(void);
    void setDefaultShipGroupName(std::string groupName);
    std::string getDefaultShipGroupName(void);

// NOTE: 
//  These AREN'T being used since we can get at the object through the pointer
//  AND the order in the vector *CAN* change at times (like the transparency sorting method)
//  so the iterator can go "bad" and not point the same object anymore. 
//         
//        std::vector<cMesh*>::iterator iteratorByUniqueID( unsigned int uniqueID_toFind, 
//                                                          const std::string &groupName, 
//                                                          bool bRecursiveSearch = true );
//        bool iteratorByUniqueID( unsigned int uniqueID_toFind, 
//                                 const std::string& groupName, 
//                                 std::vector<cMesh*>::iterator &itObjectIFound, 
//                                 bool bRecursiveSearch = true );
// 
    // If not found, return NULL (0)
    cMesh* find_pMeshByFriendlyName( const std::string& friendlyNameToFind,
                                    const std::string& groupName, 
                                    bool bRecursiveSearch = true);
    // Uses the default group name
    cMesh* find_pMeshByFriendlyName( const std::string& friendlyNameToFind,
                                    bool bRecursiveSearch = true);

    // If not found, return NULL (0)
    cMesh* find_pMeshByUniqueID( const std::string& ID_to_Find, 
                                const std::string& groupName, 
                                bool bRecursiveSearch = true);
    // **** Can NOT be updated ****
    // Returns false if not found
    bool find_MeshByFriendlyName( const std::string& friendlyNameToFind,
                                    const std::string& groupName, 
                                    cMesh &theObjectIFound, 
                                    bool bRecursiveSearch = true );    
    bool find_MeshByUniqueID( unsigned int ID_to_Find, 
                                const std::string& groupName, 
                                cMesh &theObjectIFound, 
                                bool bRecursiveSearch = true );

    // These return true if the call worked (did what was asked)
    bool addMesh( std::string groupName, cMesh* pMesh, bool bAllowReplacement = false );
    bool addMesh( cMesh* pMesh, bool bAllowReplacement = false );
    // Returns true if item is removed from the collection.
    // NOTE: This will NOT delete the object. 
    bool removeMesh( std::string groupName, cMesh* pMesh );
    bool removeMesh( std::string groupName, std::string friendlyName, bool bRemoveDuplicates = false );
    bool removeMesh( std::string groupName, unsigned int uniqueID, bool bRemoveDuplicates = false );

    // Don't keep this pointer around for long...
    std::vector<cMesh*>* get_pVecMeshesForDrawing(std::string groupName);
    std::vector<cMesh*>* get_pVecMeshesForDrawing(void);


    // These search through a vector of iShip objects.
    // If not found, return NULL (0)
    iShip* find_pShipByName( const std::string& nameToFind,
                             const std::string& groupName);
    iShip* find_pShipByName( const std::string& nameToFind );

    iShip* find_pShipByUniqueID( const std::string& ID_to_Find,
                                const std::string& groupName);
    // **** Can NOT be updated ****
    // Returns false if not found
    bool find_ShipByName( const std::string& nameToFind,
                                    const std::string& groupName, 
                                    iShip& theObjectIFound);    

    bool find_ShipByUniqueID( unsigned int uniqueID_to_Find,
                                const std::string& groupName, 
                                iShip& theObjectIFound);

    // These return true if the call worked (did what was asked)
    bool addShip( std::string groupName, iShip* pShip, bool bAllowReplacement = false );
    bool addShip( iShip* pShip, bool bAllowReplacement = false );
    bool addShips( std::string groupName, const std::vector< iShip* > &vec_pMesh, bool bAllowReplacement = false );
    // Returns true if item is removed from the collection.
    // NOTE: This will NOT delete the object. 
    bool removeShip( std::string groupName, iShip* pMesh );
    bool removeShip( std::string groupName, std::string friendlyName, bool bRemoveDuplicates = false );
    bool removeShip( std::string groupName, unsigned int uniqueID, bool bRemoveDuplicates = false );

    // Don't keep this pointer around for long...
    std::vector<iShip*>* get_pVecShipsForUpdate( std::string groupName );
    std::vector<iShip*>* get_pVecShipsForUpdate(void);

    void updateAllShips(std::string groupName, double deltaTime);
    void updateAllShips(double deltaTime);

    // This scans through all the meshes (even in ships or whatever)
    void getListOfMeshesNeeded( std::string groupName, std::vector< std::string > &vecMeshNames );
    void getListOfMeshesNeeded( std::vector< std::string > &vecMeshNames );
    
    // TODO: This won't handle cubemaps, so that's gotta be fixed...
    void getListOfTexturesNeeded( std::string groupName, std::vector< std::string > &vecTextureNames );
    void getListOfTexturesNeeded( std::vector< std::string > &vecTextureNames );



	void sortByTransparency( std::vector<cMesh*> &vecMeshes, glm::vec3 cameraEye );
	void sortByTransparency( std::string groupName, glm::vec3 cameraEye );

    std::string getLastError(bool bAndClear = true);

private:
    // These are organized by "collection" or "groups" or scenes or whatever.
    // They are for different groups of things that get dealt with or rendered at different times. 
    // For instance, when you are flying in space, you don't want to render the buildings on the planet,
    //  but once you land, you stop drawing the space ships and start drawing the buildings. 
    // 
    // In other words, you could organize this like: 
    // * The ships in orbit
    // * The meshes of terrains on the planet
    // * The objects inside a ship
    // 
    std::map< std::string /*collection name*/, std::vector< cMesh* > > mapGroupOfMeshes;
    std::map< std::string /*collection name*/, std::vector< iShip* > > mapGroupOfShips;

    // Used internally. Returns NULL (0) if not found
    // Note: DON'T keep this pointer around for long.
    std::vector<cMesh*>* m_findCollectionOfMeshes(std::string collectionName);
    std::vector<iShip*>* m_findCollectionOfShips(std::string collectionName);

    // Compares pointer values (NOT members).
    // Returns true if it found one
    bool m_findMesh(const cMesh* pMeshToFind, const std::vector<cMesh*>* p_vecMeshes);
    bool m_findShip(const iShip* pShipToFind, const std::vector<iShip*>* p_vecShips);

    // These are to help with the recursive search in the vector of child meshes
    // Returns NULL (0) if not found
    cMesh* m_find_pMeshByFriendlyName( std::string friendlyNameToFind, std::vector<cMesh*>* p_vecMeshes, bool bDoRecursiveSearch = true );
    cMesh* m_find_pMeshByUniqueID( unsigned int uniqueID, std::vector<cMesh*>* p_vecMeshes, bool bDoRecursiveSearch = true );

    // If we don't specify, then this is the default group name:
    std::string m_Default_Mesh_Group;
    std::string m_Default_Ship_Group;

    std::string m_lastError;
};

