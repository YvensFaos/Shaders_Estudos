#include "gldynamic.h"

#include "openGLWrapper.h"

#include "glbuffer.h"

//GLDynamicObject
GLDynamicObject::GLDynamicObject(void)
{
	pathName = "";
	dynamicName = "";
	index = -1;
	translate = glm::vec3(0,0,0);
	position =  glm::vec3(0,0,0);
}

GLDynamicObject::GLDynamicObject(std::string pathName, std::string dynamicName, int index, glm::vec3 translate)
{
	this->pathName = pathName;

	if(GLBufferHandler::checkForPathObject(pathName))
	{
		pathReference = GLBufferHandler::pathObjectBuffer[pathName];
	}
	else
	{
		char * pa = new char[pathName.size() + 1];
		std::copy(pathName.begin(), pathName.end(), pa);
		pa[pathName.size()] = '\0';
		pathReference = new GLPath(pa);

		GLBufferHandler::addToPathObjectBuffer(pathName, pathReference);
	}

	this->dynamicName = dynamicName;

	if(GLBufferHandler::checkForMeshHandler(dynamicName))
	{
		meshHandler = GLBufferHandler::meshHandlerBuffer[dynamicName];
	}
	else
	{
		char * da = new char[dynamicName.size() + 1];
		std::copy(dynamicName.begin(), dynamicName.end(), da);
		da[dynamicName.size()] = '\0';
		meshHandler = new GLMeshHandler(da);

		GLBufferHandler::addToMeshHandlerBuffer(dynamicName, meshHandler);
	}

	this->index = index;
	GLCameraStep* step = pathReference->getStep(index, translate);
	this->position = step->position;

	this->translate = glm::vec3(translate);
}

GLDynamicObject::~GLDynamicObject(void)
{ }


void GLDynamicObject::update(void)
{
	index++;
	if(index >= pathReference->size)
	{
		index = 0;
	}
}

void GLDynamicObject::draw(void)
{
	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	GLCameraStep* step = pathReference->getStep(index, translate);
	glUniform4f(pos, step->position.x, step->position.y, step->position.z, 0.0f);

	meshHandler->render();
}

//GLDynamic

std::vector<GLDynamicObject>* GLDynamic::generateDynamics(std::string modelPath,
														  std::string model, 
														  std::string pathPath, 
														  std::string path, int quantity, glm::vec3 translate)
{
	std::vector<GLDynamicObject>* list = new std::vector<GLDynamicObject>();
	std::string ppath = pathPath + path;
	std::string mmodel = modelPath + model;

	for(int i = 0; i < quantity; i++)
	{
		list->push_back(GLDynamicObject(ppath, mmodel, i * 10, translate));
	}

	return list;
}