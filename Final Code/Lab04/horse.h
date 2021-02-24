#pragma region INCLUDES 
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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Shader.h"
#pragma endregion INCLUDES 
#pragma once
class horse {
private:
	vec3 horseLocation;
	ModelData horseMesh;
	unsigned int vpvbo = 0;
	unsigned int vnvbo = 0;
	unsigned int vtvbo = 0;

public:
	horse();
	horse(vec3 horseLoc);
	void genhorse();
	void drawhorse(Shader shader, int width, int height, unsigned int diffuseMaphorse, unsigned int specMaphorse, bool standing);
};


