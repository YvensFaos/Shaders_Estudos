#pragma once

#include <unordered_map>
#include <string>

#include "glmesh3d.h"
#include "glbasicede.h"
#include "glcamera.h"
#include "glpath.h"
class GLBufferHandler
{
public:
	static std::unordered_map<std::string, GLMesh3D*> meshBuffer;
	static std::unordered_map<std::string, GLBasicEDE*> edeBuffer;
	static std::unordered_map<std::string, GLMeshHandler*> meshHandlerBuffer;
	static std::unordered_map<std::string, std::vector<GLCameraStep>*> pathBuffer;
	static std::unordered_map<std::string, GLPath*> pathObjectBuffer;
	//Futuramente adicionar para os dinâmicos

public:
	static bool checkForMesh(std::string identifier);
	static void addToMeshBuffer(std::string identifier, GLMesh3D* mesh);

	static bool checkForMeshHandler(std::string identifier);
	static void addToMeshHandlerBuffer(std::string identifier, GLMeshHandler* mesh);

	static bool checkForEDE(std::string identifier);
	static void addToEDEBuffer(std::string identifier, GLBasicEDE* ede);

	static bool checkForPath(std::string identifier);
	static void addToPathBuffer(std::string identifier, std::vector<GLCameraStep>* path);

	static bool checkForPathObject(std::string identifier);
	static void addToPathObjectBuffer(std::string identifier, GLPath* path);

	static void clearBuffers(void);
};