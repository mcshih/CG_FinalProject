#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>

using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawUmbreon(GLuint);
void LoadTexture(unsigned int&, const char*);
//
void DrawBall();
float BallPos = 5.0f;
void DrawCube(GLuint);

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

GLuint Normalprogram, Umbreonprogram, Cubeprogram;
GLuint VAO, VBO[3], vao_cube, vbo_cube;
unsigned int modeltexture, basistexture;
float windowSize[2] = { 600, 600 };
float angle = 0.0f;
float angle_ball = 0.0f;
float time_counter = 0.0f;
glm::vec3 WorldLightPos = glm::vec3(2, 5, 5);
glm::vec3 WorldCamPos = glm::vec3(7.5, 5.0, 7.5);
bool rotation = TRUE;

Object* model = new Object("Umbreon.obj");

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw4");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	GLuint vert = createShader("Shaders/normal.vert", "vertex");
	GLuint goem = createShader("Shaders/normal.geom", "geometry");
	GLuint frag = createShader("Shaders/normal.frag", "fragment");
	Normalprogram = createProgram(vert, goem, frag);

	vert = createShader("Shaders/Umbreon.vert", "vertex");
	frag = createShader("Shaders/Umbreon.frag", "fragment");
	Umbreonprogram = createProgram(vert, 0, frag);

	vert = createShader("Shaders/cube.vert", "vertex");
	goem = createShader("Shaders/cube.geom", "geometry");
	frag = createShader("Shaders/cube.frag", "fragment");
	Cubeprogram = createProgram(vert, goem, frag);
}

void bindbufferInit() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->positions[0]) * model->positions.size(), &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->normals[0]) * model->normals.size(), &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->texcoords[0]) * model->texcoords.size(), &model->texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//
	glGenVertexArrays(1, &vao_cube);
	glGenBuffers(1, &vbo_cube);
	glBindVertexArray(vao_cube);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(basistexture, "basis.jpg");
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 60.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawUmbreon(Normalprogram);
	DrawUmbreon(Umbreonprogram);

	DrawBall();
	DrawCube(Cubeprogram);

	time_counter += 0.01;
	if (rotation) {
		//angle += 0.2;
		angle_ball += 0.2;
		if (angle > 360) {
			angle -= 360;
		}
		if (angle_ball > 360) {
			angle_ball -= 360;
		}
	}
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	{
		glm::vec3 Zoomin = -WorldCamPos;
		glm::vec3 Left = glm::cross(glm::vec3(0, 1, 0), Zoomin);
		Left = glm::normalize(Left);
		WorldCamPos += Left;
		break;
	}
	case 'd':
	{
		glm::vec3 Zoomin = -WorldCamPos;
		glm::vec3 Right = glm::cross(Zoomin, glm::vec3(0, 1, 0));
		Right = glm::normalize(Right);
		WorldCamPos += Right;
		break;
	}
	case 's':
	{
		glm::vec3 Zoomin = -WorldCamPos;
		Zoomin = glm::normalize(Zoomin);
		WorldCamPos += Zoomin;
		break;
	}
	case 'w':
	{
		glm::vec3 Zoomout = WorldCamPos;
		Zoomout = glm::normalize(Zoomout);
		WorldCamPos += Zoomout;
		break;
	}
	case 'r':
	{
		//WorldCamPos = glm::vec3(7.5 , 5.0, 7.5);
		time_counter = 0.0f;
		break;
	}
	case 'z':
	{
		rotation = rotation==TRUE?FALSE:TRUE;
	}
	default:
	{
		break;
	}
	}
}

void idle() {
	glutPostRedisplay();
}

void DrawUmbreon(GLuint program)
{
	glUseProgram(program);

	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	//M = glm::translate(M, glm::vec3(0, 0, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	GLint nlLoc = glGetUniformLocation(program, "tc");
	float explo = time_counter - 40;
	if (time_counter * 0.1 < 4) {
		explo = 1.0f;
	}
	glUniform1f(nlLoc, explo);
	
	GLint flLoc = glGetUniformLocation(program, "falling");
	float fall = (time_counter -40)*0.2;
	if (time_counter * 0.1 < 4) {
		fall = 0.0f;
	}
	glUniform1f(flLoc, fall);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0,  3*model->fNum);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}

void DrawBall() {
	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);
	
	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z,
		0, 0, 0,
		0, 1, 0);
	
	glPushMatrix();
	if (time_counter*0.1 < 4) {
		glTranslatef(BallPos-time_counter*0.1, 0.0f, 0.0f);
		glRotatef(angle_ball, 0, 0, 1);
	}
	else {
		glTranslatef(1.0f - 4 + time_counter * 0.1, 0.0f, 0.0f);
		glRotatef(-angle_ball, 0, 0, 1);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, basistexture);
	GLUquadric* quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric, 1.0, 20, 20);
	gluDeleteQuadric(quadric);
	glActiveTexture(0);
	glPopMatrix();
}

void DrawCube(GLuint program) {
	glUseProgram(program);

	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, basistexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	glBindVertexArray(vao_cube);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}