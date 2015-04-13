#include "CAssimpModel.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <stdio.h>
#include <fstream>

bool CAssimpModel::Import3DFromFile(const std::string& pFile)
{
	Assimp::Importer importer;

	//check if file exists
    std::ifstream fin(pFile.c_str());
    if(!fin.fail()) {
        fin.close();
    }
    else{
        printf("Couldn't open file: %s\n", pFile.c_str());
        printf("%s\n", importer.GetErrorString());
        return false;
    }
 
    scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);
 
    // If the import failed, report it
    if( !scene)
    {
        printf("%s\n", importer.GetErrorString());
        return false;
    }
 
    // Now we can access the file's contents.
    printf("Import of scene %s succeeded.",pFile.c_str());
 
    aiVector3D scene_min, scene_max, scene_center;
    //get_bounding_box(&scene_min, &scene_max);
    float tmp;
    tmp = scene_max.x-scene_min.x;
    tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
    tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
    float scaleFactor = 1.f / tmp;
 
    // We're done. Everything will be cleaned up by the importer destructor
    return true;
}