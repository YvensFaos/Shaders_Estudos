#include "CAssimpModel.h"

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <stdio.h>

bool CAssimpModel::Import3DFromFile(const std::string& pFile)
{
   // Create an instance of the Importer class
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll 
  // propably to request more postprocessing than we do in this example.

  scene = importer.ReadFile( pFile, 
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  
  // If the import failed, report it
  if( !scene)
  {
    return false;
  }
  // Now we can access the file's contents. 

  // We're done. Everything will be cleaned up by the importer destructor
  return true;
}