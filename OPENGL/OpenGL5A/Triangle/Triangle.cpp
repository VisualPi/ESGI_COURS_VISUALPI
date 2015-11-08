//
// ESGI
// Triangle Basique
//

// Specifique a Windows
#ifdef _WIN32
#define GLEW_STATIC 1
#include <gl/glew.h>
#include <gl/wglew.h>

#define FREEGLUT_LIB_PRAGMAS 0
#include <gl/freeglut.h>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#endif

#include <cstdint>
#include <cstdio>
#include "../common/EsgiShader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Camera
{
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
} g_Camera;

struct Objet
{
	glm::mat4 worldMatrix;	
} g_Cube;

EsgiShader g_BasicShader;

bool Initialise() { 
	printf("Version GL : %s\n", glGetString(GL_VERSION));
	printf("Pilotes GL : %s\n", glGetString(GL_RENDERER));
	printf("Fabricant : %s\n", glGetString(GL_VENDOR));
	printf("Version GLSL : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	GLenum status = glewInit();

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.Create();

	glUseProgram(0);

#ifdef _WIN32
	wglSwapIntervalEXT(1);
#endif

	return true; 
}

void Terminate() {
	g_BasicShader.Destroy();
}

void Resize(GLint width, GLint height) {
	glViewport(0, 0, width, height);
	
	g_Camera.projectionMatrix = glm::perspectiveFov(45.f, (float)width, (float)height, 0.1f, 1000.f);
}

void Update() {	
	glutPostRedisplay();
}

void Render() 
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// alternativement on peut utiliser la nouvelle fonction glClearBufferxx()
	
	auto basicProgram = g_BasicShader.GetProgram();

	glUseProgram(basicProgram);

	g_Cube.worldMatrix = glm::mat4(1.0f);
	g_Camera.viewMatrix = glm::mat4(1.0f);
	g_Camera.viewMatrix[3].z = -2.f;
	
	static const float triangle[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.0f, 0.5f
	};
	// zero correspond ici a la valeur de layout(location=0) dans le shader basic.vs
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, triangle);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangle");

	Initialise();
	
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	
	glutMainLoop();

	Terminate();

	return 1;
}