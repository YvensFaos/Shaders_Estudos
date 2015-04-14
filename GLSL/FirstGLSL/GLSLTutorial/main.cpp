#define GLEW_STATIC

#include <GL/glew.h>

#include "GLFW/glfw3.h"

#include "math_3d.h"

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

#include <string>
  
//Define an error callback  
static void error_callback(int error, const char* description)  
{  
    fputs(description, stderr);  
    _fgetchar();  
}  
  
//Define the key input callback  
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)  
{  
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
    glfwSetWindowShouldClose(window, GL_TRUE);  
}  
  
int main( void )  
{  
    //Set the error callback  
    glfwSetErrorCallback(error_callback);  
  
    //Initialize GLFW  
    if (!glfwInit())  
    {  
        exit(EXIT_FAILURE);  
    }  
  
    //Set the GLFW window creation hints - these are optional  
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version  
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version  
    glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing  
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
  
    //Declare a window object  
    GLFWwindow* window;  
  
    //Create a window and create its OpenGL context  
    window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);  
  
    //If the window couldn't be created  
    if (!window)  
    {  
        fprintf( stderr, "Failed to open GLFW window.\n" );  
        glfwTerminate();  
        exit(EXIT_FAILURE);  
    }  
  
    //This function makes the context of the specified window current on the calling thread.   
    glfwMakeContextCurrent(window);  
  
    //Sets the key callback  
    glfwSetKeyCallback(window, key_callback);  
  
    //Initialize GLEW  
    GLenum err = glewInit();
  
    //If GLEW hasn't initialized  
    if (err != GLEW_OK)   
    {  
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));  
        return -1;  
    }  
  
	Vector3f Vertices[3];
	Vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	Vertices[1] = Vector3f(1.0f, -1.0f, 0.0f);
	Vertices[2] = Vector3f(0.0f, 1.0f, 0.0f);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint ShaderProgram = glCreateProgram();
	GLuint ShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* p[1];
	p[0] = "void main(){gl_FragColor = vec4(gl_PointCoord.x, gl_PointCoord.y, gl_FragCoord.z, 1.0);}";
	GLint Lengths[1];
	Lengths[0]= strlen(p[0]);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
	}
	glAttachShader(ShaderProgram, ShaderObj);
	glLinkProgram(ShaderProgram);

	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLchar ErrorLog[1024];
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}
	glValidateProgram(ShaderProgram);
	glUseProgram(ShaderProgram);
    //Set a background color  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  
  
    //Main Loop  
    do  
    {  
        //Clear color buffer  
        glClear(GL_COLOR_BUFFER_BIT);  
  
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDisableVertexAttribArray(0);
        //Swap buffers  
        glfwSwapBuffers(window);  
        //Get and organize events, like keyboard and mouse input, window resizing, etc...  
        glfwPollEvents();  
  
    } //Check if the ESC key had been pressed or if the window had been closed  
    while (!glfwWindowShouldClose(window));  
  
    //Close OpenGL window and terminate GLFW  
    glfwDestroyWindow(window);  
    //Finalize and clean up GLFW  
    glfwTerminate();  
  
    exit(EXIT_SUCCESS);  
}  