#pragma once
#include <glm/glm.hpp>

namespace vre {

	//Struct que armazena as informa��es dos vertices de um objeto
	struct VertexData {
		glm::vec3 position; //Posi��o
		glm::vec3 normal; //Normal
		glm::vec3 tangent; //Reflection
		glm::vec3 color; //Vertex Color
		glm::vec2 uv; //UV
	};

}