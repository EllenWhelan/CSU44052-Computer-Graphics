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
#include "Mesh.h"
#include "Shader.h"
#pragma endregion INCLUDES
#pragma once
class smallSpider
{
	private:
		//animation vars 
		GLfloat smallSpiUpperAnimation= 0.0f;
		GLfloat smallSpiLowerAnimation = 0.0f;
		GLfloat smallSpiMiddleAnimation = 0.0f;
		//movement andposition 
		GLfloat smallspiderX;
		GLfloat smallspiderY;
		GLfloat smallspiderZ;
		GLfloat walkingDirOffset;
		GLfloat faceRight;
		bool smallSpiderForward = true;
		vec3 smallSpiderinitialLocation;

		//body parts and vaos
		int NUM_LEGS = 8;
		int TOTAL_PARTS = 25;	
		ModelData* mesh_data = new ModelData[TOTAL_PARTS];
		unsigned int* smallvn_vbo = new unsigned int[TOTAL_PARTS];
		unsigned int* smallvp_vbo = new unsigned int[TOTAL_PARTS];
		unsigned int* smallvt_vbo = new unsigned int[TOTAL_PARTS];
		vec3* smallSpiderTrans = new vec3[TOTAL_PARTS];
	public:
		smallSpider();
		smallSpider(GLfloat x, GLfloat y, GLfloat z, bool forward, bool faceRight, GLfloat offset);
		void genSmallSpider();
		void draw(Shader shader, int width, int height, unsigned int diffMap);
		void updateSceneSmallSpider(float delta, bool moving);
};

