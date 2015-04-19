#pragma once
#include <GL/glew.h>
#include <iostream>
#include "meshData.h"
#include <vector>
#include <string>
#include "Program.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace vre {

	//Classe que representa um mesh
	class Mesh
	{
	public:
		explicit  Mesh(std::vector<VertexData>* vd, std::vector<unsigned int>* id, const glm::vec4& mainColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		Mesh(void);
		~Mesh(void);

		const glm::vec4& mainColor();
		void setMainColor(const glm::vec4& mainColor);
		void setMainTexture(Texture* mainTexture);
		void setNormalTexture(Texture* normalTexture);
		void setReflectionProperties(const glm::vec4& reflectionProperties);
		Texture * getMainTexture();
		void setName(const char * name);
		const char * getName();
		//virtual void draw(unsigned int programID);
		virtual void draw(Program * program);

		std::vector<VertexData> * dataPtr();
		std::vector<unsigned int> * indicesPtr();

		GLuint getVBO();
		GLuint getIndex();

		void setPosition(glm::vec3 pos);
		void setRotation(glm::vec3 rot);

		glm::mat4 getModelMatrix();
	protected:
		std::vector<VertexData> data;
		std::vector<unsigned int> indices;
		glm::vec4 _mainColor;
		glm::vec4 _reflectionProperties;
		Texture * _mainTexture;
		Texture * _normalTexture;
		unsigned int VBO;
		unsigned int VAO;
		unsigned int INDEX;
		const char * _name;

		glm::vec3 position;
		glm::vec3 rotation;

		
	};

}
