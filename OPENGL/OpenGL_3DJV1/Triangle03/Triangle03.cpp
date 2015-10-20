// Triangle01.cpp : Defines the entry point for the console application.
//

// Specifique a Windows
#if _WIN32
#include <Windows.h>
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#endif

// Entete OpenGL 
#define GLEW_STATIC 1
#include <GL/glew.h>
//#include <gl/GL.h>
//#include "GL/glext.h"

// FreeGLUT
#include "GL/freeglut.h"

#include <cstdio>
#include <cmath>

#include "../common/EsgiShader.h"
EsgiShader basicShader;

int previousTime = 0;

GLuint triangleVBO;
GLuint triangleEBO;
GLuint textureObj;

void Initialize()
{
	printf("Version Pilote OpenGL : %s\n", glGetString(GL_VERSION));
	printf("Type de GPU : %s\n", glGetString(GL_RENDERER));
	printf("Fabricant : %s\n", glGetString(GL_VENDOR));
	printf("Version GLSL : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	
	GLenum error = glewInit();
	if (error != GL_NO_ERROR) {
		// TODO
	}

	for (int index = 0; index < numExtensions; ++index)
	{
		printf("Extension[%d] : %s\n", index, glGetStringi(GL_EXTENSIONS, index));
	}

	basicShader.LoadVertexShader("basic.vs");
	basicShader.LoadFragmentShader("basic.fs");
	basicShader.Create();

	static const float g_Triangle[] = {
		-0.8f, 0.8f, 1.0f,
		0.0f, -0.8f, 0.5f,
		0.8f, 0.8f, 0.0f
	};

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, g_Triangle, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, nullptr, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*9, g_Triangle);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	static const unsigned short indicesTriangle[] = { 0, 1, 2 };
	glGenBuffers(1, &triangleEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*3, indicesTriangle, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	static unsigned char image[] = { 
		255, 0, 0, 255, 0, 255, 0, 255, 
		0, 0, 255, 255, 255, 255, 0, 255 
	};
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);

	previousTime = glutGet(GLUT_ELAPSED_TIME);
}

void Terminate()
{
	glDeleteTextures(1, &textureObj);
	glDeleteBuffers(1, &triangleVBO);

	basicShader.Destroy();
}

void Render()
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glClearColor(0.f, 0.5f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint program = basicShader.GetProgram();
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE,3*sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);
	GLint intensityLocation = glGetAttribLocation(program, "a_intensity");
	glVertexAttribPointer(intensityLocation, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(intensityLocation);

	// variables uniformes (constantes) durant le rendu de la primitive
	GLint offsetLocation = glGetUniformLocation(program, "u_offset");
	glUniform3f(offsetLocation, 0.0f, 0.0f, 0.0f);
	GLint colorLocation = glGetUniformLocation(program, "u_constantColor");
	glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture0");
	glUniform1i(textureLocation, 0);

	glDrawArrays(GL_POINTS, 0, 3);

	//static unsigned short indicesTriangle[] = { 0, 1, 2 };
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta/1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);
	
	glUniform3f(offsetLocation, cos(time), sin(time), 0.0f);
	glUniform4f(colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr/*indicesTriangle*/);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Basic");

#ifdef FREEGLUT
	// Note: glutSetOption n'est disponible qu'avec freeGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
				  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	Initialize();

	glutDisplayFunc(Render);

	glutMainLoop();

	Terminate();

	return 0;
}

