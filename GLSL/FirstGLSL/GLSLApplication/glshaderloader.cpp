#include "glshaderloader.h"

unsigned long GLShaderLoader::getFileLength(std::ifstream& file)
{
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);
    
    return len;
}

GLchar* GLShaderLoader::loadshader(char* filename, unsigned long* len)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file)
	{
		return nullptr;
	}
    
	unsigned long lenv = 0;
	lenv = GLShaderLoader::getFileLength(file);
	(*len) = lenv;

	if (lenv == 0)
	{
		return nullptr;   // Error: Empty File 
	}
    
	GLchar* shaderSource = new char[lenv + 1];
   
	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
	shaderSource[lenv] = 0; 

	unsigned int i=0;
	while (file.good())
	{
		shaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
		{
			i++;
		}
	}
    
	shaderSource[i] = 0;  // 0-terminate it at the correct position
    
	file.close();
      
	return shaderSource; // No Error
}


void GLShaderLoader::unloadshader(GLubyte** ShaderSource)
{
   if (*ShaderSource != 0)
   {
     delete[] *ShaderSource;
   }
   *ShaderSource = 0;
}