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

GLuint cubeVBO;
GLuint cubeIBO;
GLuint textureObj;

#include "cube.h"

/*
struct Camera
{
	
} g_Camera;*/

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
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	basicShader.LoadVertexShader("basic.vs");
	basicShader.LoadFragmentShader("basic.fs");
	basicShader.Create();

	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, g_cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * 2 * 3, g_cubeIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	previousTime = glutGet(GLUT_ELAPSED_TIME);
}

void Terminate()
{
	glDeleteTextures(1, &textureObj);
	glDeleteBuffers(1, &cubeIBO);
	glDeleteBuffers(1, &cubeVBO);

	basicShader.Destroy();
}

// ---

void Identity(float *matrix)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 1.0f; matrix[5] = 1.0f; matrix[10] = 1.0f; matrix[15] = 1.0f;
}

void Orthographic(float *matrix, float L, float R, float T, float B, float N, float F)
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = 2.f / (R-L); 
	matrix[5] = 2.f / (T-B); 
	matrix[10] = -2.f / (F-N);
	matrix[12] = -(R+L) / (R-L);
	matrix[13] = -(T+B) / (T-B);
	matrix[14] = -(F+N) / (F-N);
	matrix[15] = 1.f;
}

void Perspective(float *matrix, float FOV, float width, float height, float N, float F)
{
	memset(matrix, 0, sizeof(float) * 16);
	float aspect = width / height;

	float xymax = N * tan(FOV * (3.141592f / 180.f) * 0.5f);
	float ymin = -xymax;
	float xmin = -xymax;
	width = xymax - xmin;
	height = xymax - ymin;
	float depth = F - N;
	float q = -(F+N) / depth;
	float qn = -2.0f * (F*N) / depth;
	
	float w = 2.0f * N / width;
	w = w / aspect;
	float h = 2.0f * N / height;

	matrix[0] = w;
	matrix[5] = h;
	matrix[10] = q;
	matrix[11] = -1.f;
	matrix[14] = qn;
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
	glClearColor(0.f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint program = basicShader.GetProgram();
	glUseProgram(program);

	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	// passage des attributs de sommet au shader

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	GLint positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

	// variables uniformes (constantes) durant le rendu de la primitive
	
	float projection[16];
	//Orthographic(projection, 0, w, h, 0, -1.f, 1.f);
	Perspective(projection, 45.f, w, h, 0.1f, 1000.f);
	//Identity(projection);
	GLint projLocation = glGetUniformLocation(program, "u_projectionMatrix");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, projection);
	
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta/1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);
	
	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[14] = -7.0f;
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);

	float worldTransform[16];
	Identity(worldTransform);
	Translate(worldTransform, 5.f * sin(time), 0.0f, 0.0F);
	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_SHORT, 0);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Cube Basique");

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

