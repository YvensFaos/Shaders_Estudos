#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

int main()
{
	char* model = "ravine.obj";

	//char* path = "E:/Repositorios/Shaders_Estudos/Models/";
	char* path = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	//char* path = "X:/Yvens Rebou�as/GIT/Shaders_Estudos/Models/";

	char sFilePath[512];
	sprintf(sFilePath, "%s%s", path, model);

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

	printf("Loaded! %s\n", sFilePath);
	int exit = 0;
	scanf("%d", &exit);

	return exit;
}