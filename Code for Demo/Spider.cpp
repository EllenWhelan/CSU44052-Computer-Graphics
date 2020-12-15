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

Spider::Spider() {}

Spider::Spider(GLfloat x, GLfloat y, GLfloat z, bool forwardInput) {
	spiderX = x; 
	spiderY = y;
	spiderZ = z;
	forward = forwardInput;
}
void Spider::genObject() {
	//filenames 
	const char* meshNames[25] = { "meshes/0.obj", "meshes/1.obj", "meshes/2.obj", "meshes/3.obj", "meshes/4.obj", "meshes/5.obj", "meshes/6.obj", "meshes/7.obj", "meshes/8.obj", "meshes/9.obj","meshes/10.obj",
	"meshes/11.obj", "meshes/12.obj", "meshes/13.obj", "meshes/14.obj", "meshes/15.obj", "meshes/16.obj", "meshes/17.obj", "meshes/18.obj", "meshes/19.obj","meshes/20.obj",
	"meshes/21.obj", "meshes/22.obj", "meshes/23.obj", "meshes/24.obj" };
	std::string matrixNames[25] = { "meshes/0.matrix", "meshes/1.matrix", "meshes/2.matrix", "meshes/3.matrix", "meshes/4.matrix", "meshes/5.matrix", "meshes/6.matrix", "meshes/7.matrix", "meshes/8.matrix", "meshes/9.matrix","meshes/10.matrix",
	"meshes/11.matrix", "meshes/12.matrix", "meshes/13.matrix", "meshes/14.matrix", "meshes/15.matrix", "meshes/16.matrix", "meshes/17.matrix", "meshes/18.matrix", "meshes/19.matrix","meshes/20.matrix",
	"meshes/21.matrix", "meshes/22.matrix", "meshes/23.matrix", "meshes/24.matrix" };

	//matrix file stuff 
	std::string matFile = matrixNames[0];
	std::ifstream Matfile(matFile);
	if (Matfile.good()) {
		std::string sLine;
		std::getline(Matfile, sLine);
		std::istringstream iss(sLine);
		float x, y, z;
		iss >> x >> y >> z;
		trans[0] = vec3(-x, -z, y);
	}

	Mesh m;
	mesh_data[0] = m.load_mesh(meshNames[0]);

	vp_vbo[0] = 0;
	glGenBuffers(1, &vp_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mVertices[0], GL_STATIC_DRAW);

	vn_vbo[0] = 0;
	glGenBuffers(1, &vn_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mNormals[0], GL_STATIC_DRAW);



	for (int i = 1; i < TOTAL_PARTS; i++) {
		//matrix file stuff 
		std::string matFile = matrixNames[i];
		std::ifstream Matfile(matFile);
		if (Matfile.good()) {
			std::string sLine;
			std::getline(Matfile, sLine);
			std::istringstream iss(sLine);
			float x, y, z;
			iss >> x >> y >> z;
			trans[i] = vec3(-x, -z, y);
		}
		mesh_data[i] = m.load_mesh(meshNames[i]);

		vp_vbo[i] = 0;
		glGenBuffers(1, &vp_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mVertices[0], GL_STATIC_DRAW);

		vn_vbo[i] = 0;
		glGenBuffers(1, &vn_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mNormals[0], GL_STATIC_DRAW);
	}
	unsigned int vao = 0; 
	glBindVertexArray(vao);

};

void Spider::draw(Shader shader, int width, int height, unsigned int diffMap) { 
	
	shader.use();
	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;// glGetAttribLocation(shader.ID, "vertex_position");
	GLuint loc2 = 1;// glGetAttribLocation(shader.ID, "vertex_normal");
	GLuint loc3 = 2; // glGetAttribLocation(shader.ID, "vertex_texture");

	// draw the Body 
	mat4 model = identity_mat4();
	if (!forward)model = rotate_y_deg(model, 180.0f); //face spider backwards if moving backwards 
	model = translate(model, vec3(spiderX, spiderY, spiderZ)); //move spider to location - spiderZ changes here for forwards and back wards movement
	shader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, diffMap);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[0]);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[0]);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[0]);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// update uniforms & draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data[0].mPointCount);

	//iterate through legs to draw them 
	mat4 parentModel = model;
	int index = 3;
	while (index <= 24) {
		mat4 upperChild = identity_mat4();
		mat4 middleChild = identity_mat4();
		mat4 lowerChild = identity_mat4();

		//calculate rotations for animation 
		float upperChildRotation = 0.0f;
		float middleChildRotation = 0.0f;
		float lowerChildRotation = 0.0f; 
		float initialX = 0.0f; 
		if (index== 3 || index == 9 || index == 18 || index == 24) {
			upperChildRotation = initialX + rotate_y;
			middleChildRotation = (-upperChildRotation / 3.0f) - mid_y;
			lowerChildRotation = sway_y;
		}
		else if (index == 6 || index == 12 || index == 15 || index == 21) {
			upperChildRotation = initialX - rotate_y;
			middleChildRotation = (-upperChildRotation / 3.0f) - mid_y;
			lowerChildRotation = - sway_y;
		}
		//rotations first - all parts start with joint in middle -animation
		upperChild = rotate_x_deg(upperChild, upperChildRotation);
		middleChild= rotate_x_deg(middleChild,middleChildRotation);
		lowerChild= rotate_x_deg(lowerChild, lowerChildRotation);
		//translations
		//do upper translations 
		upperChild = translate(upperChild, trans[index]);//to proper location
		upperChild = parentModel * upperChild;

		//do all middle child translations
		middleChild = translate(middleChild, trans[index - 1]-trans[index]);//to proper location
		middleChild = upperChild * middleChild;

		//do all lowerChild translations
		lowerChild = translate(lowerChild, trans[index - 2]-trans[index-1]);//to proper location
		lowerChild = middleChild * lowerChild;

		
		//draw upper leg		
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		// update uniforms & draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, upperChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index].mPointCount);

		//draw middle leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index-1]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index-1]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-1]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		// update uniforms & draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, middleChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index-1].mPointCount);
	
		//draw lower leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index-2]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index-2]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-2]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		// update uniforms & draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lowerChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index-2].mPointCount);

		//reset 
		index = index + 3; //increase index to access next upper leg 
	}
	
}
void Spider::updateSceneSpider(float delta) {
	//animation of spiders legs 
	float currentTime = timeGetTime();
	rotate_y = cos(currentTime / 240.0f) * 30.0f;
	sway_y = sin(currentTime / 240.0f) * 20.0f;
	mid_y = sin(currentTime / 240.0f) * 20.0f;

	//moving spiders forwards/backwards through scene 
	if (forward)spiderZ += 0.0251f;
	else if (!forward)spiderZ -= 0.0251f;

}


