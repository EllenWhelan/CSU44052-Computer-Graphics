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
#pragma endregion INCLUDES

#pragma once
class LightBox
{
private:
public:
	//vertices
  
	GLfloat scaleFactor; 
	vec3 initalPos;
	unsigned int lightBoxVAO;
	LightBox();
	LightBox(GLfloat scale, vec3 initialLoc);
	void genLightBox();
	void drawLightBox(Shader lightSourceShader, int width, int height, vec3 lightPosition); //set light properties in here 

};

