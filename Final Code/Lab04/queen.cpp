#include "queen.h"
queen::queen::queen()
{
}

queen::queen(vec3 queenLoc)
{
	queenLocation = queenLoc;
}

//function to load queen mesh and create vao 
void queen::genQueen()
{
	Mesh mesh;
	queenMesh = mesh.load_mesh("meshes/queen.obj");
	
	glGenBuffers(1, &vpvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vpvbo);
	glBufferData(GL_ARRAY_BUFFER, queenMesh.mPointCount * sizeof(vec3), &queenMesh.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vnvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vnvbo);
	glBufferData(GL_ARRAY_BUFFER, queenMesh.mPointCount * sizeof(vec3), &queenMesh.mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vtvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vtvbo);
	glBufferData(GL_ARRAY_BUFFER, queenMesh.mPointCount * sizeof(vec2), &queenMesh.mTextureCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //release bidning 
}

//functino to draw queen oieces 
void queen::drawQueen(Shader shader, int width, int height, unsigned int diffuseMapQueen, unsigned int specMapQueen)
{
	shader.use();
	//loc in shader 
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;
	GLuint loc2 = 1;
	GLuint loc3 = 2; 

	mat4 model = identity_mat4();
	model = scale(model, vec3(10.0f, 10.0f, 10.0f)); //scale to proper position 
	model = rotate_x_deg(model, -90); //rotate to proper orientatoion
	model = translate(model, queenLocation);//translare to correct location
	shader.setMat4("model", model);
	//texture mapping
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMapQueen);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specMapQueen);

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
	glDrawArrays(GL_TRIANGLES, 0, queenMesh.mPointCount);
}
