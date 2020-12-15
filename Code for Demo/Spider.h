#ifndef SPIDER_HEADER
#define SPIDER_HEADER
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
class Spider
{
	private: 
		//animation vars 
		GLfloat rotate_y = 0.0f;
		GLfloat sway_y = 0.0f;
		GLfloat mid_y = 0.0f;
		//movement
		GLfloat spiderX; 
		GLfloat spiderY;
		GLfloat spiderZ; 
		bool forward=true; 
		
		int NUM_LEGS = 8;
		int TOTAL_PARTS = 25;
		vec3 initialLocation;
		//GLfloat forward_z = 0.0f; 
		//GLfloat bob = 0.0f; 
		//bool reset = false; 

		ModelData* mesh_data = new ModelData[TOTAL_PARTS];
		unsigned int* vn_vbo = new unsigned int[TOTAL_PARTS];
		unsigned int* vp_vbo = new unsigned int[TOTAL_PARTS];
		unsigned int* vt_vbo = new unsigned int[TOTAL_PARTS];
		vec3* trans = new vec3[TOTAL_PARTS];
	public:
		Spider();
		Spider(GLfloat x, GLfloat y, GLfloat z, bool forward);
		void genObject();
		void draw(Shader shader, int width, int height, unsigned int diffMap); //Shader shaderProgram, Camera camera
		void updateSceneSpider(float delta);
	
};

#endif