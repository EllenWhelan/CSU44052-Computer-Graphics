# pragma region INCLUDES 
// Windows includes (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "Spider.h"
#include "Mesh.h"
#include "Camera.h"
#include "GroundPlane.h"
#include <random>
#include "Shader.h"
#include "LightBox.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include <filesystem>
#pragma endregion INCLUDES


#pragma region GLOBALS 
using namespace std;
GLuint shaderProgramID; //shader stuff 
Shader objectShader; 
Shader lightSrcShader; 
int width = 800;
int height = 600;

LightBox lightBox1; //light src stuff 
vec3 lightPos(0.0f, 100.0f, 2.0f);
vec3 pointLightPositions[] = {
	  vec3(0.0f,  0.2f,  2.0f),vec3(200.0f, -3.3f, -4.0f),vec3(-200.0f,  2.0f, -12.0f),vec3(0.0f,  0.0f, -3.0f), 
	  vec3(0.0f,  0.2f,  400.0f),vec3(200.0f, -3.3f, 400.0f),vec3(-200.0f,  2.0f, 400.0f),vec3(0.0f,  0.0f, 400.0f),
	  vec3(0.0f,  0.2f,  -400.0f),vec3(200.0f, -3.3f, -400.0f),vec3(-200.0f,  2.0f, -12.0f),vec3(0.0f,  0.0f, -400.0f),

};
//vec3(0.7f,  0.2f,  2.0f),vec3(2.3f, -3.3f, -4.0f),vec3(-4.0f,  2.0f, -12.0f),vec3(0.0f,  0.0f, -3.0f)

Camera camera; //camera stuff 
float lastx = width / 2.0f;
float lasty = height / 2.0f;

const int NUM_SPIDERS = 6; //spider stuff 
const int NUM_SPIDER_SPECIES = 4; 

Spider spiders1[NUM_SPIDERS]; //red ones 
Spider spiders2[NUM_SPIDERS];//orange ones 
Spider spiders3[NUM_SPIDERS];//green ones
Spider spiders4[NUM_SPIDERS];//blue ones 

GLfloat spidersX1[NUM_SPIDERS] = { -150.0f, 50.0f, 150.0f, -250.0f, -100.0f, 100.0f};
GLfloat spidersX2[NUM_SPIDERS] = { -250.0f, -50.0f, 100.0f, -50.0f, 50.0f, 150.0f };
GLfloat spidersX3[NUM_SPIDERS] = { -150.0f, -50.0f, 50.0f, 150.0f, -150.0f, 100.0f };
GLfloat spidersX4[NUM_SPIDERS] = { -100.0f, 100.0f, -250.0f, -150.0f, -100.0f, 100.0f };

GLfloat spidersY = 0.0f;
GLfloat spidersZ1[NUM_SPIDERS]= { -300.0f,-300.0f,-300.0f, 300.0f, 300.0f,300.0f};
GLfloat spidersZ2[NUM_SPIDERS] = { -100.0f,-100.0f,-100.0f, 200.0f, 100.0f,100.0f };
GLfloat spidersZ3[NUM_SPIDERS] = { -100.0f,-200.0f,-200.0f, -100.0f, 100.0f,200.0f };
GLfloat spidersZ4[NUM_SPIDERS] = { -200.0f,-300.0f,100.0f, 200.0f, 100.0f,100.0f };

bool directions1[NUM_SPIDERS] = { true, true, true, false, false, false};
bool directions2[NUM_SPIDERS] = { false, true, false, true, true, true};
bool directions3[NUM_SPIDERS] = { true, true, true, true, true, false };
bool directions4[NUM_SPIDERS] = { false, false, false, true, false, false };

//ground planes 
const int NUM_GROUNDS = 9;

// a b c
// d e f
// g h i

GroundPlane grounds[NUM_GROUNDS];
vec3 groundLocations[NUM_GROUNDS] = { vec3(-1000.0f, -20.0f, -1000.0f), vec3(0.0f, -20.0f, -1000.0f),vec3(1000.0f, -20.0f, -1000.0f),
									  vec3(-1000.0f, -20.0f, 0.0f), vec3(0.0f, -20.0f, 0.0f), vec3(1000.0f, -20.0f, 0.0f),
									  vec3(-1000.0f, -20.0f, 1000.0f), vec3(0.0f, -20.0f, 1000.0f), vec3(1000.0f, -20.0f, 1000.0f) };


//texture maps 
unsigned int diffuseMapGround; 
unsigned int diffuseMapWalls; 
unsigned int diffuseMapSpider1; 
unsigned int diffuseMapSpider2;
unsigned int diffuseMapSpider3;
unsigned int diffuseMapSpider4;

#pragma endregion GLOBALS 


#pragma region SET_UP_FUNCTIONS
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void setUpLights() {
	//set light properties of objects 
	  // directional light
	objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	objectShader.setVec3("dirLight.ambient", 0.075f, 0.075f, 0.075f);//0.05
	objectShader.setVec3("dirLight.diffuse",0.3f, 0.3f, 0.3f); //0.4 colour of light red light 
	objectShader.setVec3("dirLight.specular",0.75f, 0.75f, 0.75f); //0,5
	// spotLight
	objectShader.setVec3("spotLight.position", camera.Position);
	objectShader.setVec3("spotLight.direction", camera.Front);
	objectShader.setVec3("spotLight.ambient", 20.0f, 20.0f, 20.0f); //0.0
	objectShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("spotLight.constant", 1.0f);
	objectShader.setFloat("spotLight.linear", 0.001);
	objectShader.setFloat("spotLight.quadratic", 0.005);
	objectShader.setFloat("spotLight.cutOff", cos((12.5f * M_PI) / 180)); //rad = degree * (pi / 180
	objectShader.setFloat("spotLight.outerCutOff", cos((15.0f * M_PI) / 180));

	// point light 1
	objectShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	objectShader.setVec3("pointLights[0].ambient", 20.0f, 20.0f, 20.0f); //0.05
	objectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[0].constant", 1.0f);
	objectShader.setFloat("pointLights[0].linear", 0.001); //0.09 smaller attenuation bigger light 
	objectShader.setFloat("pointLights[0].quadratic", 0.005); //0.032
	// point light 2
	objectShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	objectShader.setVec3("pointLights[1].ambient", 10.0f, 10.0f, 10.0f);//0.05
	objectShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[1].constant", 1.0f);
	objectShader.setFloat("pointLights[1].linear", 0.001);
	objectShader.setFloat("pointLights[1].quadratic", 0.005);
	// point light 3
	objectShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	objectShader.setVec3("pointLights[2].ambient", 10.0f, 10.0f, 10.0f);
	objectShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[2].constant", 1.0f);
	objectShader.setFloat("pointLights[2].linear", 0.001);
	objectShader.setFloat("pointLights[2].quadratic", 0.005);
	// point light 4
	objectShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	objectShader.setVec3("pointLights[3].ambient", 20.0f, 20.0f, 20.0f); //0.05
	objectShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[3].constant", 1.0f);
	objectShader.setFloat("pointLights[3].linear", 0.001); //0.09 smaller attenuation bigger light 
	objectShader.setFloat("pointLights[3].quadratic", 0.005); //0.032
	// point light 5
	objectShader.setVec3("pointLights[4].position", pointLightPositions[4]);
	objectShader.setVec3("pointLights[4].ambient", 10.0f, 10.0f, 10.0f);//0.05
	objectShader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[4].constant", 1.0f);
	objectShader.setFloat("pointLights[4].linear", 0.001);
	objectShader.setFloat("pointLights[4].quadratic", 0.005);
	// point light 6
	objectShader.setVec3("pointLights[5].position", pointLightPositions[5]);
	objectShader.setVec3("pointLights[5].ambient", 10.0f, 10.0f, 10.0f);
	objectShader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[5].constant", 1.0f);
	objectShader.setFloat("pointLights[5].linear", 0.001);
	objectShader.setFloat("pointLights[5].quadratic", 0.005);
	// point light 7
	objectShader.setVec3("pointLights[6].position", pointLightPositions[6]);
	objectShader.setVec3("pointLights[6].ambient", 20.0f, 20.0f, 20.0f); //0.05
	objectShader.setVec3("pointLights[6].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[6].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[6].constant", 1.0f);
	objectShader.setFloat("pointLights[6].linear", 0.001); //0.09 smaller attenuation bigger light 
	objectShader.setFloat("pointLights[6].quadratic", 0.005); //0.032
	// point light 8
	objectShader.setVec3("pointLights[7].position", pointLightPositions[7]);
	objectShader.setVec3("pointLights[7].ambient", 10.0f, 10.0f, 10.0f);//0.05
	objectShader.setVec3("pointLights[7].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[7].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[7].constant", 1.0f);
	objectShader.setFloat("pointLights[7].linear", 0.001);
	objectShader.setFloat("pointLights[7].quadratic", 0.005);
	// point light 9
	objectShader.setVec3("pointLights[8].position", pointLightPositions[8]);
	objectShader.setVec3("pointLights[8].ambient", 10.0f, 10.0f, 10.0f);
	objectShader.setVec3("pointLights[8].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[8].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[8].constant", 1.0f);
	objectShader.setFloat("pointLights[8].linear", 0.001);
	objectShader.setFloat("pointLights[8].quadratic", 0.005);	
}

void setupTextures() {
	//load in textures
	string path = "textures/wall.jpg";
	diffuseMapGround = loadTexture(path.c_str());
	path = "textures/spiderFurGrey.jpg";
	diffuseMapSpider1 = loadTexture(path.c_str());
	path = "textures/spiderFur.jpg";
	diffuseMapSpider2 = loadTexture(path.c_str());
	path = "textures/spiderFurBrown.jpg";
	diffuseMapSpider3 = loadTexture(path.c_str());
	path = "textures/spiderFurNavy.jpg";
	diffuseMapSpider4 = loadTexture(path.c_str());
}

void setupShaders() {
	objectShader = Shader("objectVS.txt", "objectFS.txt");// Set up the shaders
	//spiderShader = Shader("simpleVertexShader.txt", "simpleFragmentShader.txt");
	//lightSrcShader = Shader("lightSourceVS.txt", "lightSourceFS.txt"); //set up light box)
}

void generateSpiders() {
	//crowd of spiders 
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders1[i] = Spider(spidersX1[i], spidersY, spidersZ1[i], directions1[i]);
		spiders1[i].genObject();

	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders2[i] = Spider(spidersX2[i], spidersY, spidersZ2[i], directions2[i]);
		spiders2[i].genObject();

	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders3[i] = Spider(spidersX3[i], spidersY, spidersZ3[i], directions3[i]);
		spiders3[i].genObject();

	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders4[i] = Spider(spidersX4[i], spidersY, spidersZ4[i], directions4[i]);
		spiders4[i].genObject();

	}
}

void generateGround() {

	for (int i = 0; i < NUM_GROUNDS; i++) {
		grounds[i] = GroundPlane(groundLocations[i]);
		grounds[i].genGround();
	}
}

#pragma endregion SET_UP_FUNCTIONS

void updateScene() {

	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.001f;
	last_time = curr_time;
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders1[i].updateSceneSpider(delta);
	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders2[i].updateSceneSpider(delta);
	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders3[i].updateSceneSpider(delta);
	}
	for (int i = 0; i < NUM_SPIDERS; i++) {
		spiders4[i].updateSceneSpider(delta);
	}
	// Draw the next frame
	glutPostRedisplay();

}

void display() {
	//GENERAL SET UP 
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f); //chenge from 0.5 for forst 3
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 projection = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 view = camera.GetViewMatrix();

	//DRAW FLOOR 
	objectShader.use();
	//material properties
	objectShader.setInt("material.diffuse", 0);
	objectShader.setInt("material.specular", 1);
	objectShader.setFloat("material.shininess", 62.0f);//objectShader.setVec3("light.position", lightPos);
	// view/projection transformations
	objectShader.setVec3("viewPos", camera.Position);
	objectShader.setMat4("projection", projection);
	objectShader.setMat4("view", view);
	setUpLights();
	//DRAW FLOORS 
	for (int i = 0; i < NUM_GROUNDS; i++) {
		grounds[i].drawGround(objectShader, width, height, diffuseMapGround);
	}

	//DRAW SPIDERS  
	objectShader.setFloat("material.shininess", 100.0f); //shiny spiders 
	for (int i = 0; i < NUM_SPIDER_SPECIES; i++) {
		for (int j = 0; j < NUM_SPIDERS; j++) {
			if (i == 0)spiders1[j].draw(objectShader, width, height, diffuseMapSpider1);
			if (i == 1)spiders2[j].draw(objectShader, width, height, diffuseMapSpider2);
			if (i == 2)spiders3[j].draw(objectShader, width, height, diffuseMapSpider3);
			if (i == 3)spiders4[j].draw(objectShader, width, height, diffuseMapSpider4);
		}
	}
	glutSwapBuffers();
}

void init()
{ 
	setupShaders();
	setupTextures();
	generateSpiders(); //create crowd of spiders 
	generateGround();//create ground planes
	camera = Camera(); //create camera for scene 
}


#pragma region HANDLE_USER_INPUT
void keypress(unsigned char key, int x, int y) {
	if (key == 'w') {
			camera.ProcessKeyboard(FORWARD, 0.1);
	}
	if (key == 'a') {
		camera.ProcessKeyboard(LEFT, 0.1);
	}
	if (key == 'd') {
		camera.ProcessKeyboard(RIGHT, 0.1);
	}
	if (key == 's') {
		camera.ProcessKeyboard(BACKWARD, 0.1);
	}

	
	
}
void mouse(int button, int state, int x, int y) {
	if (button == 4)camera.ProcessMouseScroll(1.0f);
	if (button == 3)camera.ProcessMouseScroll(-1.0f);
}
void mouseCallback(int x, int y) {
	float xoffset = x - lastx;
	float yoffset = lasty - y;
	lastx = x;
	lasty = y;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
#pragma endregion HANDLE_USER_INPUT


int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Creepy Scene");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseCallback);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
