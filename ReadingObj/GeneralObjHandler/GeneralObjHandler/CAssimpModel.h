#ifndef __C_ASSIMP_MODEL__
#define __C_ASSIMP_MODEL__

#include <string>

struct aiScene;

class CAssimpModel 
{ 
private:
	const aiScene* scene;

public: 
	CAssimpModel() {} 
	~CAssimpModel(){}

public: 
	bool Import3DFromFile(const std::string& pFile);
};

#endif