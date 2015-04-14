#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

int main()
{
	char* sFilePath = "path";

	Assimp::Importer importer; 
	const aiScene* scene = importer.ReadFile( sFilePath,  
                               aiProcess_CalcTangentSpace       |  
                               aiProcess_Triangulate            | 
                               aiProcess_JoinIdenticalVertices  | 
                               aiProcess_SortByPType); 

	if(!scene) 
	{ 
		printf("Couldn't load model\n");
		return -1; 
	}

	return 0;
}