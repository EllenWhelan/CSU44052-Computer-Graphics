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
class pawn{
	private:
		vec3 pawnLocation;
		ModelData pawnMesh;
		unsigned int vpvbo = 0;
		unsigned int vnvbo = 0;
		unsigned int vtvbo = 0;

	public:
		pawn();
		pawn(vec3 pawnLoc);
		void genPawn();
		void drawPawn(Shader shader, int width, int height, unsigned int diffuseMapPawn, unsigned int specMapPawn, bool standing);

};

