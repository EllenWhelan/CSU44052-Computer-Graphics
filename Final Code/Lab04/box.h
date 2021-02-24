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
//#define STB_IMAGE_IMPLEMENTATION
//#include "../stb_image.h"
#include <filesystem>
#include "skybox.h"
#include "smallSpider.h"
#include "pawn.h"
#include "horse.h"
#include "queen.h"
#pragma endregion INCLUDES
#pragma once
class box
{
private:
	vec3 boxLocation;
	ModelData boxMesh;
	unsigned int boxvpvbo = 0;
	unsigned int boxvnvbo = 0;
	unsigned int boxvtvbo = 0;
public:
	box();
	box(vec3 boxloc);
	void genBox();
	void drawBox(Shader shader, int width, int height, unsigned int diffMap);
};



