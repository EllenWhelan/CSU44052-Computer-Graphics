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
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#include <filesystem>
#include "skybox.h"
#include "smallSpider.h"
#include "pawn.h"
#include "horse.h"
#include "queen.h"
#include "box.h"
#include "Cube.h"
#pragma endregion INCLUDES


#pragma region GLOBALS 
using namespace std;
//---------------------------------SHADERS-------------------------------------
Shader objectShader; 
Shader skyboxShader;

int width = 800;
int height = 600;

bool moving;
//-------------------------------LIGHTS--------------------------------------------------
vec3 lightPos(0.0f, 100.0f, 2.0f);
vec3 pointLightPositions[] = {
	  vec3(0.0f,  0.2f,  2.0f),vec3(200.0f, -3.3f, -4.0f),vec3(-200.0f,  2.0f, -12.0f),vec3(0.0f,  0.0f, -3.0f), 
	  vec3(0.0f,  0.2f,  400.0f),vec3(200.0f, -3.3f, 400.0f),vec3(-200.0f,  2.0f, 400.0f),vec3(0.0f,  0.0f, 400.0f),
	  vec3(0.0f,  0.2f,  -400.0f),vec3(200.0f, -3.3f, -400.0f),vec3(-200.0f,  2.0f, -12.0f),vec3(0.0f,  0.0f, -400.0f),

};
//light colours 
vec3 redLight = vec3(1.0f, 0.0f, 0.0f);
vec3 blueLight = vec3(0.0f, 0.0f, 1.0f);
vec3 greenLight = vec3(0.0f, 1.0f, 0.0f);
vec3 normalLight = vec3(0.3f, 0.3f, 0.3f);
vec3 currentLight;
//brightness
vec3 currentBrightness;
vec3 lowBrightness = vec3(0.0075f, 0.0075f, 0.0075f);
vec3 midBrightness = vec3(0.075f, 0.075f, 0.075f);
vec3 highBrightness = vec3(0.75f, 0.75f, 0.75f);


//------------------------------------CAMERA-------------------------------------------------------
Camera camera; 
float lastx = width / 2.0f;
float lasty = height / 2.0f;

//---------------------------------------SPIDERS-----------------------------------------------------
const int NUM_SPIDERS = 50;
Spider spiders[50];
const int NUM_SMALL = 300;
smallSpider smallSpiders[NUM_SMALL]; 

//spider speeds
GLfloat lowSpeed = 0.0151f;
GLfloat midSpeed = 0.05;
GLfloat highSpeed = 0.5;
GLfloat currentSpeed;

//----------------------------------------GROUND PLANES --------------------------------------------
const int NUM_GROUNDS = 9;

// a b c
// d e f
// g h i
GroundPlane grounds[NUM_GROUNDS];
vec3 groundLocations[NUM_GROUNDS] = { vec3(-1000.0f, -20.0f, -1000.0f), vec3(0.0f, -20.0f, -1000.0f),vec3(1000.0f, -20.0f, -1000.0f),
									  vec3(-1000.0f, -20.0f, 0.0f), vec3(0.0f, -20.0f, 0.0f), vec3(1000.0f, -20.0f, 0.0f),
									  vec3(-1000.0f, -20.0f, 1000.0f), vec3(0.0f, -20.0f, 1000.0f), vec3(1000.0f, -20.0f, 1000.0f) };


//--------------------------------------TEXTURE MAPS ----------------------------------------------------------
unsigned int diffuseMapGround; 
unsigned int diffuseMapWalls; 
unsigned int diffuseMapSpider1; 
unsigned int diffuseMapSpider2;
unsigned int diffuseMapWhite;
unsigned int diffuseMapBlack;
unsigned int specMapWhite; 
unsigned int specMapBlack;

//-------------------------------------SKYBOX--------------------------------------------------------------------
skybox sky;
unsigned int skyBoxTextureMap;

//---------------------------------------CHESS PIECES------------------------------------------------------------
queen queens[2];
vec3 queenLoc[2] = { vec3(-400,-20.0f, -100.0f), vec3(150.0f, -20.0f, 0.0f) };
bool queenCols[2] = { true, false }; //true = white 

horse horses[4];
vec3 horseLoc[4] = { vec3(-200.0f,-20.0f,100.0f), vec3(-50.0f, -20.0f, -250.0f), vec3(100.0f,-20.0f,-200.0f), vec3(200.0f,-20.0f,-150.0f)};
bool horseCols[4] = {false, false, true, true};
bool standingHorses[4] = { true, true,false,true };

pawn pawns[10];
vec3 pawnLoc[10] = {vec3(-400.0f, -40.0f, -300.0f),vec3(-300.0f,-40.0f, -300.0f),vec3(-200.0f, -40.0f, -300.0f),vec3(-150.0f,-40.0f, -200.0f),vec3(-100.0f, -40.0f, -250.0f),vec3(-50.0f,-40.0f, -50.0f),vec3(75.0f,-40.0f,-100.0f),vec3(150.0f,-40.0f,-300.0f),vec3(200.0f, -40.0f,-150.0f),vec3(-400.0f,-40.0f, -100.0f)};
bool pawnCols[10] = { true, false,false, true,false, true, false, false, true, true };
bool standingPawns[10] = { true,true,true,false,true,true,false, false,true,true };

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

unsigned int loadSkyTexture()
{
   	std::string faces[] =
	{
		"skybox/redsky1.jpg",
		"skybox/redsky2.jpg",
		"skybox/redsky3.jpg",
		"skybox/redsky4.jpg",
		"skybox/redsky5.jpg",
	   "skybox/redsky6.jpg"
	};
    
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    
}

void setUpSkyBox() {
	//set up skybox 
	sky = skybox();
	sky.genSkybox();
	skyBoxTextureMap = loadSkyTexture();
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	objectShader.use();
}

void setUpLights() {
	//set light properties of objects 
	  // directional light
	objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	objectShader.setVec3("dirLight.ambient",currentBrightness);//0.05
	objectShader.setVec3("dirLight.diffuse", currentLight); //0.4 colour of light red light 
	objectShader.setVec3("dirLight.specular",0.75f, 0.75f, 0.75f); //0,5
	// spotLight
	objectShader.setVec3("spotLight.position", camera.Pos);
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
	objectShader.setVec3("pointLights[0].ambient", 5.0f, 5.0f, 5.0f); //0.05 20.0f
	objectShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	objectShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("pointLights[0].constant", 1.0f);
	objectShader.setFloat("pointLights[0].linear", 0.001); //0.09 smaller attenuation bigger light 
	objectShader.setFloat("pointLights[0].quadratic", 0.05); //0.032
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
	string path = "textures/chessboard.jpg";
	diffuseMapGround = loadTexture(path.c_str());
	path = "textures/spiderFur.jpg";
	diffuseMapSpider1 = loadTexture(path.c_str());
	path = "textures/blackWood.jpg";
	diffuseMapSpider2 = loadTexture(path.c_str());
	path = "textures/brownmarble.jpg";
	diffuseMapBlack = loadTexture(path.c_str());
	path = "textures/marble.jpg";
	diffuseMapWhite = loadTexture(path.c_str());
	path = "textures/brownmarblespec.jpg";
	specMapBlack = loadTexture(path.c_str());
	path = "textures/marblespec.jpg";
	specMapWhite = loadTexture(path.c_str());
	
}

void setupShaders() {
	objectShader = Shader("objectVS.txt", "objectFS.txt");// Set up the shaders
	skyboxShader = Shader("skyboxVS.txt", "skyboxFS.txt"); //set up skybox shader 
}

void generateSpiders() {
	GLfloat xpos, ypos, zpos;
	bool dir;
	ypos = -10.0f;
	for (int i = 0; i < 50; i++) {
		if (i % 2 == 0)dir = true;
		else dir = false;
		zpos = rand() % 300 + (-300);
		bool found = false;
		while (!found) {
			xpos = rand() % 1000 + (-500);
			if ((int)xpos % 50 == 0)found = false; //number is divisble by 50 ao avoid
			else found = true;
		}
		spiders[i] = Spider(xpos, ypos, zpos, dir);
		spiders[i].genObject();

	}	
}

void generateSmallSpiders() {
	bool dir;
	GLfloat xpos;
	GLfloat ypos = -15.0f;
	GLfloat zpos;
	bool face;
	GLfloat offset;
	for (int i = 0; i < NUM_SMALL; i++) {
		if (i % 2 == 0) {
			dir = true;
			face = false;
			offset = 0 - (rand() % 35 + 0);
		}
		else {
			dir = false;
			face = true;
			offset = rand() % 35 + 0;
		}
		zpos = rand() % 300 + (-300); //pick z locations randomly 
		xpos = rand() % 1000 + (-500);
		smallSpiders[i] = smallSpider(xpos, ypos, zpos, dir, face, offset);
		smallSpiders[i].genSmallSpider();
		
		
	}

}

void generateGround() {

	for (int i = 0; i < NUM_GROUNDS; i++) {
		grounds[i] = GroundPlane(groundLocations[i]);
		grounds[i].genGround();
	}
}

void generateChessPieces() {
	for (int i = 0; i < 2; i++) {
		queens[i] = queen(queenLoc[i]);
		queens[i].genQueen();
	}
	for (int i = 0; i < 4; i++) {
		horses[i] = horse(horseLoc[i]);
		horses[i].genhorse();
	}
	for (int i = 0; i < 10; i++) {
		pawns[i] = pawn(pawnLoc[i]);
		pawns[i] = pawn(pawnLoc[i]);
		pawns[i].genPawn();
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
	//UPDATE SPIDERS
	for (int i = 0; i < 50; i++) {
		spiders[i].updateSceneSpider(delta, currentSpeed, moving);
	}
	//UPDATE SMALLER SPIDERS 
	for (int i = 0; i < NUM_SMALL; i++) {
		smallSpiders[i].updateSceneSmallSpider(delta, moving);
	}
	// Draw the next frame
	glutPostRedisplay();

}

void display() {
	//GENERAL SET UP 
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.50f, 0.50f, 0.50f, 0.5f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mat4 projection = perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	mat4 view = camera.GetViewMatrix();

	//DRAW FLOOR 
	objectShader.use();
	objectShader.setInt("material.diffuse", 0);
	objectShader.setInt("material.specular", 1);
	objectShader.setFloat("material.shininess", 62.0f);
	objectShader.setVec3("viewPos", camera.Pos);
	objectShader.setMat4("projection", projection);
	objectShader.setMat4("view", view);
	setUpLights();
	for (int i = 0; i < NUM_GROUNDS; i++) {
		grounds[i].drawGround(objectShader, width, height, diffuseMapGround);
	}

	//DRAW SPIDERS  
	objectShader.use();
	objectShader.setFloat("material.shininess", 100.0f); //shiny spiders 
	for (int i = 0; i < 50; i++) {
		spiders[i].draw(objectShader, width, height, diffuseMapSpider1);
	}
	//DRAW SMALL SPIDERS 
	objectShader.setFloat("material.shininess", 100.0f);
	for (int i = 0; i < NUM_SMALL; i++) {
		smallSpiders[i].draw(objectShader, width, height, diffuseMapSpider2);
	}

	//DRAW CHESS PIECES 
	objectShader.setFloat("material.shininess", 300.0f);
	for (int i = 0; i < 2; i++) {
		if (queenCols[i] == true)queens[i].drawQueen(objectShader, width, height, diffuseMapWhite,specMapWhite);
		else queens[i].drawQueen(objectShader, width, height, diffuseMapBlack, specMapBlack);
	}
	for (int i = 0; i < 4; i++) {
		if (horseCols[i] == true)horses[i].drawhorse(objectShader, width, height, diffuseMapWhite, specMapWhite, standingHorses[i]);
		else horses[i].drawhorse(objectShader, width, height, diffuseMapBlack, specMapBlack,standingHorses[i]);
	}
	for (int i = 0; i < 10; i++) {
		if (pawnCols[i] == true)pawns[i].drawPawn(objectShader, width, height, diffuseMapWhite, specMapWhite, standingPawns[i]);
		else pawns[i].drawPawn(objectShader, width, height, diffuseMapBlack, specMapBlack, standingPawns[i]);
	}

	
	
	//draw skybox 
	skyboxShader.use();
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	view = identity_mat4();
	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);
	sky.drawSkyBox(skyboxShader, skyBoxTextureMap);
	
	//reset 
	glutSwapBuffers();
}

void init()
{ 
	setupShaders();
	setupTextures();
	setUpSkyBox();
	generateSpiders(); 
	generateSmallSpiders(); 
	generateGround();
	generateChessPieces();
	camera = Camera();
	//set up bools and vars for keyboard interaction
	currentLight = normalLight;
	currentSpeed = midSpeed;
	currentBrightness = midBrightness;
	moving = true;

}


#pragma region HANDLE_USER_INPUT
void keypress(unsigned char key, int x, int y) {
	if (key == 'w') { //move cam forward
			camera.ProcessKeyboard(FORWARD, 0.1);
	}
	if (key == 'a') {//mpve cam left
		camera.ProcessKeyboard(LEFT, 0.1);
	}
	if (key == 'd') {//move cam right
		camera.ProcessKeyboard(RIGHT, 0.1);
	}
	if (key == 's') {//move cam backward
		camera.ProcessKeyboard(BACKWARD, 0.1);
	}
	//keys for changing lights in scene 
	if (key == 'r') {//turn light red 
		currentLight = redLight;
	}
	if (key == 'g') {//urn light green 
		currentLight = greenLight;
	}
	if (key == 'b') {//turn light blue 
		currentLight = blueLight;
	}
	if (key == 'n') {//return light to normal 
		currentLight = normalLight;
	}
	//keys for spider speed adjustments
	if (key == '1') { //lowest speed
		currentSpeed = lowSpeed;
	}
	if (key == '2') {//middle speed
		currentSpeed = midSpeed;
	}
	if (key == '3') {//fastest speed
		currentSpeed = highSpeed;
	}
	//keys for brightness level of scene
	if (key == 'i') {//increase brightness 
		currentBrightness = highBrightness;
	}
	if (key == 'k') {//lower brightness 
		currentBrightness = lowBrightness;
	}
	if (key == 'h') {//return lighting and speed to default 
		currentBrightness = midBrightness;
		currentLight = normalLight;
		currentSpeed = midSpeed;
	}
	//keys for pausing movement 
	if (key == 'v') {//start/stop movement of spiders 
		if (moving)moving = false;
		else moving = true;
	}
	 
	
	
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
	glutCreateWindow("Queens Gambit Inspired Creey Scene");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	//glutMouseFunc(mouse);
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
