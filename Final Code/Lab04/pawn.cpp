#include "pawn.h"

pawn::pawn()
{
}

pawn::pawn(vec3 pawnLoc)
{
	pawnLocation = pawnLoc;
}

//function to load in pawn mesh and create vao 
void pawn::genPawn()
{
	Mesh mesh;
	pawnMesh = mesh.load_mesh("meshes/untitled.obj");
	
	glGenBuffers(1, &vpvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vpvbo);
	glBufferData(GL_ARRAY_BUFFER, pawnMesh.mPointCount * sizeof(vec3), &pawnMesh.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vnvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vnvbo);
	glBufferData(GL_ARRAY_BUFFER, pawnMesh.mPointCount * sizeof(vec3), &pawnMesh.mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vtvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vtvbo);
	glBufferData(GL_ARRAY_BUFFER, pawnMesh.mPointCount * sizeof(vec2), &pawnMesh.mTextureCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //release binding 
}

//function to draw pawn 
void pawn::drawPawn(Shader shader, int width, int height, unsigned int diffuseMapPawn, unsigned int specMapPawn, bool standing)
{
	shader.use();
	//loc in shader 
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;
	GLuint loc2 = 1;
	GLuint loc3 = 2; 

	mat4 model = identity_mat4();
	model = scale(model, vec3(5.0f, 5.0f, 5.0f)); //scale to correct size 
	if (!standing) { //if lying down rotate and translate t correct position 
		model = rotate_z_deg(model, -100);
		model = translate(model, vec3(pawnLocation.v[0], 10.0f, pawnLocation.v[2]));
	}
	else model = translate(model, pawnLocation);//if standing translate to correct pos
	shader.setMat4("model", model);
	//texture mapping
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMapPawn);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specMapPawn);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vpvbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vnvbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vtvbo);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// draw 
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, pawnMesh.mPointCount);
}
