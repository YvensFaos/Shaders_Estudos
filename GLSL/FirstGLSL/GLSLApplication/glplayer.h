#ifndef __GL_PLAYER__
#define __GL_PLAYER__

class GLPlayer
{
private:
	char dynamicName[256];
	char* title;

public:
	GLFWwindow* actualWindow;

public:
	GLPlayer(void);
	~GLPlayer(void);

	void initializeGLPlayer(void);
};

#endif