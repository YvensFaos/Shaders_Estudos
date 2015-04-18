#include "Mesh.h"


using namespace vre;

Mesh::Mesh(std::vector<VertexData>* vd,std::vector<unsigned int>* id, const glm::vec4& mainColor)
{
	
	data=*vd;
	indices=*id;
	_mainColor = mainColor;
	_reflectionProperties = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	_mainTexture = NULL;
	_normalTexture = NULL;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	
	glBufferData(GL_ARRAY_BUFFER,data.size()*sizeof(VertexData), &data[0],GL_STATIC_DRAW);
	
	glGenBuffers(1,&INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,INDEX);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	
	int vertex = 0;  //Vertice
	int normal = 1; //Normal
	int color = 2; //Cor
	int UV = 3; //UV
	int tangent = 4; //UV

	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(9*sizeof(float)));
	
	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(12*sizeof(float)));

	/*
	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);
	*/

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

Mesh::Mesh(void)
{
}

Mesh::~Mesh(void)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteBuffers(1,&INDEX);
}

const glm::vec4& Mesh::mainColor()
{
	return _mainColor;
}

void  Mesh::setMainColor(const glm::vec4& mainColor)
{
	_mainColor = mainColor;
}

void Mesh::setMainTexture(Texture* mainTexture)
{
	_mainTexture = mainTexture;
}

void Mesh::setNormalTexture(Texture* normalTexture)
{
	_normalTexture = normalTexture;
}

void Mesh::setReflectionProperties(const glm::vec4& reflectionProperties)
{
	_reflectionProperties = reflectionProperties;
}

Texture * Mesh::getMainTexture()
{
	return _mainTexture;
}


void Mesh::draw(Program * program)
{
	//attribute vec3 vertex
	/*
	int vertex = program->getAttribLocation("vertex");
	int normal = program->getAttribLocation("normal");
	int color = program->getAttribLocation("color");
	int UV = program->getAttribLocation("UV");
	int mainColor = program->getUniformLocation("mainColor");
	*/
	
	//Seta a cor principal
	if(!program->isShadowOnly()) {
		int mainColor = program->getUniformLocation("mainColor");
		program->setUniform(mainColor, _mainColor);
	}

	int reflectionProperties = program->getUniformLocation("reflectionProperties", false);//
	if (reflectionProperties >= 0) {

		program->setUniform(reflectionProperties, _reflectionProperties);//
	}

	int useNormalmap = program->getUniformLocation("useNormalmap", false);//
	if (useNormalmap >= 0) {
		if (_normalTexture != NULL) {
			program->setUniform(useNormalmap, 1);
			_normalTexture->bind(GL_TEXTURE1);
		} else {
			program->setUniform(useNormalmap, 0);
		}//
	}

	//Seta a textura
	if(_mainTexture) {
		_mainTexture->bind(GL_TEXTURE0);
	}

	if (program->getUniformLocation("model", false) != -1) {
		program->setUniform(program->getUniformLocation("model"), this->getModelMatrix());
	}

	//Desenha o objeto	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);//


	//glBindVertexArray(0);
	
	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	
}

std::vector<VertexData> * Mesh::dataPtr()
{
	return &data;
}

std::vector<unsigned int> * Mesh::indicesPtr()
{
	return &indices;
}

void Mesh::setName(const char * name)
{
	_name = name;
}

const char * Mesh::getName()
{
	return _name;
}

GLuint Mesh::getVBO()
{
	return VBO;
}

GLuint Mesh::getIndex()
{
	return INDEX;
}

void Mesh::setPosition(glm::vec3 pos)
{
	this->position = pos;
}

void Mesh::setRotation(glm::vec3 rot)
{
	this->rotation = rot;
}

glm::mat4 Mesh::getModelMatrix()
{
	glm::mat4 model = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0, 1, 0));
	
	model = glm::rotate(model, this->rotation.x, glm::vec3(1, 0, 0));
	model = glm::rotate(model, this->rotation.y, glm::vec3(0, 1, 0));
	model = glm::rotate(model, this->rotation.z, glm::vec3(0, 0, 1));
	model = glm::translate(model, this->position);
	return model;
}