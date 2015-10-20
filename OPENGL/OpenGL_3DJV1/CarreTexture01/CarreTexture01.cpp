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

// STB
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

#include <cstdio>
#include <cmath>

#include "../common/EsgiShader.h"
EsgiShader basicShader;

int previousTime = 0;

GLuint carreVBO;
GLuint carreEBO;
GLuint textureObj;

struct Sprite
{
	float x, y;
} g_Sprite[100];

struct Camera
{
	float x, y;
} g_Camera;

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

	static const float g_Carre[] = {
		// x,y			intens.	s,t
		-32.0f, 32.0f,	1.0f,	0.0f, 1.0f,
		-32.0f, -32.0f,	0.5f,	0.0f, 0.0f,
		32.0f, -32.0f,	0.5f,	1.0f, 0.0f,
		32.0f, 32.0f,		0.0f,	1.f, 1.f,
	};

	glGenBuffers(1, &carreVBO);
	glBindBuffer(GL_ARRAY_BUFFER, carreVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*20, g_Carre, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*9, nullptr, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*9, g_Triangle);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	static const unsigned short indicesCarre[] = { 0, 1, 2, 2, 3, 0 };
	glGenBuffers(1, &carreEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carreEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*6, indicesCarre, GL_STATIC_DRAW);
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

	static const int worldSize = 3000;
	for (int index = 0; index < 100; ++index)
	{
		g_Sprite[index].x = (float(rand())/RAND_MAX - 0.5f) * worldSize/2;
		g_Sprite[index].y = (float(rand())/RAND_MAX - 0.5f) * worldSize/2;
	}

	previousTime = glutGet(GLUT_ELAPSED_TIME);
}

void Terminate()
{
	glDeleteTextures(1, &textureObj);
	glDeleteBuffers(1, &carreVBO);

	basicShader.Destroy();
}

void Orthographic(float *matrix, float L, float R, float T, float B, float N, float F)
{
#if 0
	const float mat4[] = {
		2.f / (R-L), 0.0f, 0.0f, 0.0f,
		0.0f, 2.f / (T-B), 0.0f, 0.0f,
		0.0f, 0.0f, -2.f / (F-N), 0.0f,
		-(R+L)/(R-L), -(T+B)/(T-B), -(F+N)/(F-N), 1.0f
	};
	memcpy(matrix, mat4, sizeof(float) * 16);
#else
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 2.f / (R-L); 
	matrix[5] = 2.f / (T-B); 
	matrix[10] = -2.f / (F-N);
	matrix[12] = -(R+L) / (R-L);
	matrix[13] = -(T+B) / (T-B);
	matrix[14] = -(F+N) / (F-N);
	matrix[15] = 1.f;
#endif
}

void Translate(float *matrix, float tx, float ty, float tz = 0.f)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 1.f; 
	matrix[5] = 1.f; 
	matrix[10] = 1.f;
	matrix[12] = tx;
	matrix[13] = ty;
	matrix[14] = tz;
	matrix[15] = 1.f;
}

void Render()
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glClearColor(0.f, 0.5f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint program = basicShader.GetProgram();
	glUseProgram(program);

	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	float projection[16];
	//Orthographic(projection, -w/2, w/2, -h/2, h/2, -1.f, 1.f);
	Orthographic(projection, 0, w, h, 0, -1.f, 1.f);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);

	glBindBuffer(GL_ARRAY_BUFFER, carreVBO);

	int TailleVertex = 5*sizeof(float);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE,TailleVertex, 0);
	glEnableVertexAttribArray(positionLocation);
	GLint intensityLocation = glGetAttribLocation(program, "a_intensity");
	glVertexAttribPointer(intensityLocation, 1, GL_FLOAT, GL_FALSE, TailleVertex, (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(intensityLocation);
	GLint texCoordsLocation = glGetAttribLocation(program, "a_texCoords");
	glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, GL_FALSE, TailleVertex, (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(texCoordsLocation);

	// variables uniformes (constantes) durant le rendu de la primitive
	GLint offsetLocation = glGetUniformLocation(program, "u_offset");
	glUniform3f(offsetLocation, 0.0f, 0.0f, 0.0f);
	GLint colorLocation = glGetUniformLocation(program, "u_constantColor");
	glUniform4f(colorLocation, 1.0f, 0.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	GLint textureLocation = glGetUniformLocation(program, "u_texture0");
	glUniform1i(textureLocation, 0);

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta/1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);
	
	float viewTransform[16];
	Translate(viewTransform, -time*60.f, 0.f);
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	//glUniform3f(offsetLocation, cos(time), sin(time), 0.0f);
	glUniform4f(colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, carreEBO);

	float worldTranslate[16];
	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	for (int index = 0; index < 100; index++) {
		Translate(worldTranslate, g_Sprite[index].x, g_Sprite[index].y);	
		glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTranslate);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr/*indicesTriangle*/);

	};

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

