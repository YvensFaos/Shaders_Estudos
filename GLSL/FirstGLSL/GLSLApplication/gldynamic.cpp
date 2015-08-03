#include "gldynamic.h"

#include "openGLWrapper.h"

#include "glbuffer.h"
#include "glmathhelper.h"

//GLDynamicObject
GLDynamicObject::GLDynamicObject(void)
{
	pathName = "";
	dynamicName = "";
	visible = false;
	index = -1;
	translate = glm::vec3(0, 0, 0);
}

GLDynamicObject::GLDynamicObject(std::string pathName, std::string dynamicName, int index, glm::vec3 translate, glm::vec3 scale)
{
	this->pathName = pathName;
	visible = false;

	if (GLBufferHandler::checkForPathObject(pathName))
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

	if (GLBufferHandler::checkForMeshHandler(dynamicName))
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

	this->translate = glm::vec3(translate);
	this->scale = glm::vec3(scale);
}

GLDynamicObject::~GLDynamicObject(void)
{ }

void GLDynamicObject::update(void)
{
	index++;
	if (index >= pathReference->size)
	{
		index = 0;
	}
}

void GLDynamicObject::draw(void)
{
	if (visible)
	{
		GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
		GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
		GLCameraStep* step = pathReference->getStep(index, translate);
		glUniform4f(pos, step->position.x, step->position.y, step->position.z, 0.0f);
		glUniform4f(sca, scale.x, scale.y, scale.z, 1.0f);

		meshHandler->render();
	}

	//Reseta o visible para falso sempre que é desenhado
	visible = false;
}

void GLDynamicObject::draw(glm::vec3 pos)
{
	if (visible)
	{
		GLint poss = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
		GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
		glUniform4f(poss, pos.x, pos.y, pos.z, 0.0f);
		glUniform4f(sca, scale.x, scale.y, scale.z, 1.0f);

		meshHandler->render();
	}

	//Reseta o visible para falso sempre que é desenhado
	visible = false;
}

void GLDynamicObject::getBounds(glm::vec3 bounds[2])
{
	GLCameraStep* step = pathReference->getStep(index, translate);
	bounds[0] = meshHandler->min + step->position;
	bounds[1] = meshHandler->max + step->position;
}

void GLDynamicObject::drawBox(void)
{
	GLint loc = 0;
	if (visible)
	{
		loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
		glUniform4f(loc, 0.008f, 0.24f, 0.74f, 1.0f);
	}
	else
	{
		loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
		glUniform4f(loc, 0.74f, 0.24f, 0.008f, 1.0f);
	}

	GLCameraStep* step = pathReference->getStep(index, translate);
	GLAABB::drawAABB(meshHandler->min, meshHandler->max, step->position);
}

//GLDynamic

std::vector<GLDynamicObject>* GLDynamic::generateDynamics(std::string modelPath,
	std::string model,
	std::string pathPath,
	std::string path, int quantity, glm::vec3 translate, glm::vec3 scale)
{
	std::vector<GLDynamicObject>* list = new std::vector<GLDynamicObject>();
	std::string ppath = pathPath + path;
	std::string mmodel = modelPath + model;

	for (int i = 0; i < quantity; i++)
	{
		list->push_back(GLDynamicObject(ppath, mmodel, 100 + i * 10, translate, scale));
	}

	return list;
}