#include "glbuffer.h"

std::unordered_map<std::string, GLMesh3D*> GLBufferHandler::meshBuffer;
std::unordered_map<std::string, GLBasicEDE*> GLBufferHandler::edeBuffer;
std::unordered_map<std::string, GLMeshHandler*> GLBufferHandler::meshHandlerBuffer;
std::unordered_map<std::string, std::vector<GLCameraStep>*> GLBufferHandler::pathBuffer;

bool GLBufferHandler::checkForMesh(std::string identifier)
{
	return !(meshBuffer.find(identifier) == meshBuffer.end());
}

bool GLBufferHandler::checkForEDE(std::string identifier)
{
	return !(edeBuffer.find(identifier) == edeBuffer.end());
}

bool GLBufferHandler::checkForPath(std::string identifier)
{
	return !(pathBuffer.find(identifier) == pathBuffer.end());
}

bool GLBufferHandler::checkForMeshHandler(std::string identifier)
{
	return !(meshHandlerBuffer.find(identifier) == meshHandlerBuffer.end());
}

void GLBufferHandler::addToMeshBuffer(std::string identifier, GLMesh3D* mesh)
{
	meshBuffer.insert(std::unordered_map<std::string, GLMesh3D*>::value_type(identifier, mesh));
}

void GLBufferHandler::addToMeshHandlerBuffer(std::string identifier, GLMeshHandler* mesh)
{
	meshHandlerBuffer.insert(std::unordered_map<std::string, GLMeshHandler*>::value_type(identifier, mesh));
}

void GLBufferHandler::addToEDEBuffer(std::string identifier, GLBasicEDE* ede)
{
	edeBuffer.insert(std::unordered_map<std::string, GLBasicEDE*>::value_type(identifier, ede));
}

void GLBufferHandler::addToPathBuffer(std::string identifier, std::vector<GLCameraStep>* path)
{
	pathBuffer.insert(std::unordered_map<std::string, std::vector<GLCameraStep>*>::value_type(identifier, path));
}

void GLBufferHandler::clearBuffers(void)
{
	edeBuffer.clear();
	meshBuffer.clear();
	pathBuffer.clear();
}

