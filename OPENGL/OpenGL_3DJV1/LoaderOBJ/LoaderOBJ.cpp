//
// LoaderOBJ.cpp : Defines the entry point for the console application.
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
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb-master/stb_image.h"

#include "../common/tiny_obj_loader.h"

#include <cstdio>
#include <cmath>

#include <vector>
#include <string>

#include "../common/EsgiShader.h"
EsgiShader basicShader;

int previousTime = 0;

GLuint cubeVBO;
GLuint cubeIBO;
GLuint cubeElementCount;

GLuint objectVBO;
GLuint objectIBO;
GLuint objectElementCount;

GLenum elementType = GL_UNSIGNED_INT;

GLuint textureObj;

/*
struct Camera
{
	
} g_Camera;*/

void InitCube()
{
	std::string inputfile = "cube.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());
	const std::vector<unsigned int> &indices = shapes[0].mesh.indices;
	const std::vector<float> &positions = shapes[0].mesh.positions;
	const std::vector<float> &normals = shapes[0].mesh.normals;
	const std::vector<float> &texcoords = shapes[0].mesh.texcoords;

	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeIBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	cubeElementCount = indices.size();
}

void PrepareCube(GLuint program)
{
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	auto positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
}

void CleanCube()
{
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeIBO);
}

void InitObject()
{
	std::string inputfile = "suzanne.obj";
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err = tinyobj::LoadObj(shapes, materials, inputfile.c_str());
	const std::vector<unsigned int> &indices = shapes[0].mesh.indices;
	const std::vector<float> &positions = shapes[0].mesh.positions;
	const std::vector<float> &normals = shapes[0].mesh.normals;
	const std::vector<float> &texcoords = shapes[0].mesh.texcoords;

	glGenBuffers(1, &objectVBO);
	glGenBuffers(1, &objectIBO);
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	objectElementCount = indices.size();
}

void PrepareObject(GLuint program)
{
	glBindBuffer(GL_ARRAY_BUFFER, objectVBO);
	auto positionLocation = glGetAttribLocation(program, "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
}

void CleanObject()
{
	glDeleteBuffers(1, &objectVBO);
	glDeleteBuffers(1, &objectIBO);
}

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


	previousTime = glutGet(GLUT_ELAPSED_TIME);

	InitCube();
	InitObject();
}

void Terminate()
{
	glDeleteTextures(1, &textureObj);
	CleanObject();
	CleanCube();

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

#define M_PI 3.141592f

void Perspective(float *m, float fov, float width_, float height_, float znear, float zfar)
{
	memset(m, 0, sizeof(float) * 16);
	float aspect = width_ / height_;

	float xymax = znear * tan(fov * 0.5f * M_PI/180.0f);

	float ymin = -xymax;
	float xmin = -xymax;
 
	float width = xymax - xmin;
	float height = xymax - ymin;
 
	float depth = zfar - znear;
	float q = -(zfar + znear) / depth;
	float qn = -2 * (zfar * znear) / depth;
 
	float w = 2 * znear / width;
	w = w / aspect;
	float h = 2 * znear / height;
 
	m[0]  = w; m[1]  = 0; m[2]  = 0; m[3]  = 0;
	m[4]  = 0; m[5]  = h; m[6]  = 0; m[7]  = 0;
	m[8]  = 0; m[9]  = 0; m[10] = q; m[11] = -1;
	m[12] = 0; m[13] = 0; m[14] = qn; m[15] = 0;
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	GLuint program = basicShader.GetProgram();
	glUseProgram(program);

	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	// variables uniformes (constantes) durant le rendu de la primitive
	
	float projection[16];
	//Orthographic(projection, -w/2, w/2, -h/2, h/2, -1.f, 1.f);
	//Orthographic(projection, 0, w, h, 0, -1.f, 1.f);
	Perspective(projection, 45.f, w, h, 0.01f, 1000.f);
	
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int delta = currentTime - previousTime;
	previousTime = currentTime;
	static float time = 1.f;
	time += delta/1000.f;
	GLint timeLocation = glGetUniformLocation(program, "u_time");
	glUniform1f(timeLocation, time);
	
	float viewTransform[16];
	Identity(viewTransform);
	viewTransform[14] = -7.f;

	float worldTransform[16];
	Identity(worldTransform);
	worldTransform[12] = sin(time) * 5.f;

	GLint projectionLocation = glGetUniformLocation(program, "u_projectionMatrix");
	GLint viewLocation = glGetUniformLocation(program, "u_viewMatrix");
	GLint worldLocation = glGetUniformLocation(program, "u_worldMatrix");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewTransform);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldTransform);

	PrepareCube(program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO);
	glDrawElements(GL_TRIANGLES, cubeElementCount, elementType, 0);

	PrepareObject(program);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectIBO);
	glDrawElements(GL_TRIANGLES, objectElementCount, elementType, 0);

	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OBJ Loader");

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

