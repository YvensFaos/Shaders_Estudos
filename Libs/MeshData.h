#pragma once
#include <glm/glm.hpp>

namespace vre {

	//Struct que armazena as informações dos vertices de um objeto
	struct VertexData {
		glm::vec3 position; //Posição
		glm::vec3 normal; //Normal
		glm::vec3 tangent; //Reflection
		glm::vec3 color; //Vertex Color
		glm::vec2 uv; //UV
	};

}