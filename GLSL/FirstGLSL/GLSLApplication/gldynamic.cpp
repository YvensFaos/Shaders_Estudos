#include "gldynamic.h"

#include "glbuffer.h"

//GLDynamicObject
GLDynamicObject::GLDynamicObject(void)
{
	pathName = "";
	dynamicName = "";
	index = -1;
	translate = glm::vec3(0,0,0);
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
		pathReference = new GLPath(pa);

		GLBufferHandler::addToPathObjectBuffer(pathName, pathReference);
	}

	this->dynamicName = dynamicName;
	this->index = index;
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

}

//GLDynamic

std::vector<GLDynamicObject>* GLDynamic::generateDynamics(std::string modelPath, std::string model, std::string pathPath, std::string path, int quantity, glm::vec3 translate)
{
	std::vector<GLDynamicObject>* list = new std::vector<GLDynamicObject>();
	std::string ppath = pathPath + path;
	std::string mmodel = modelPath + model;

	for(int i = 0; i < quantity; i++)
	{
		list->push_back(GLDynamicObject(ppath, mmodel, i * 3, translate));
	}

	return list;
}