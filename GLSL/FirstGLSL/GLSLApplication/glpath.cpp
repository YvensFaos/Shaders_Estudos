#include "glpath.h"

#include "edfile.h"

GLPath::GLPath(void)
{
	size = -1;
}

GLPath::GLPath(char* pathfile)
{
	readPathFile(pathfile);
}

GLPath::GLPath(char* pathfilePath, char* pathfileName)
{
	char pathfile[256];
	sprintf(pathfile, "%s%s", pathfilePath, pathfileName);
	readPathFile(pathfile);
}


GLPath::~GLPath(void)
{ }

void GLPath::readPathFile(char* pathfile)
{
	EDFileReader reader = EDFileReader(pathfile);
	this->size = reader.readLnInt();

	steps = new std::vector<GLCameraStep>();

	for(int i = 0; i < size; i++)
	{
		float posx = reader.readLnFloat();
		float posy = reader.readLnFloat();
		float posz = reader.readLnFloat();

		float lookx = reader.readLnFloat();
		float looky = reader.readLnFloat();
		float lookz = reader.readLnFloat();

		float upx = reader.readLnFloat();
		float upy = reader.readLnFloat();
		float upz = reader.readLnFloat();

		float fov = reader.readLnFloat();

		steps->push_back(GLCameraStep(glm::vec3(posx,posy,posz), glm::vec3(upx,upy,upz), glm::vec3(lookx,looky,lookz), fov));
	}

	reader.close();
}

GLCameraStep* GLPath::getStep(int index)
{
	return &steps->at(index);
}

GLCameraStep* GLPath::getStep(int index, glm::vec3 translate)
{
	GLCameraStep* nstep = new GLCameraStep(steps->at(index));
	nstep->position += translate;

	return nstep;
}