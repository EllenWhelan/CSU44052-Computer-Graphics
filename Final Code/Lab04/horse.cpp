
#include "horse.h"

horse::horse()
{
}

horse::horse(vec3 horseLoc)
{
	horseLocation = horseLoc;
}

//function to load in mesh for object and create vao 
void horse::genhorse()
{
	Mesh mesh;
	horseMesh = mesh.load_mesh("meshes/horse.obj");

	glGenBuffers(1, &vpvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vpvbo);
	glBufferData(GL_ARRAY_BUFFER, horseMesh.mPointCount * sizeof(vec3), &horseMesh.mVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vnvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vnvbo);
	glBufferData(GL_ARRAY_BUFFER, horseMesh.mPointCount * sizeof(vec3), &horseMesh.mNormals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vtvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vtvbo);
	glBufferData(GL_ARRAY_BUFFER, horseMesh.mPointCount * sizeof(vec2), &horseMesh.mTextureCoords[0], GL_STATIC_DRAW);

	glBindVertexArray(0); //unbind current vao
}

//function to drawhorse 
void horse::drawhorse(Shader shader, int width, int height, unsigned int diffuseMaphorse, unsigned int specMaphorse, bool standing)
{
	shader.use();
	//loc in shader 
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;
	GLuint loc2 = 1;
	GLuint loc3 = 2; 

	mat4 model = identity_mat4();
	model = scale(model, vec3(10.0f, 10.0f, 10.0f)); //scale to proper soze 
	model = rotate_x_deg(model, -90); //rotate model to upright position
	model = rotate_y_deg(model, 180);
	if (!standing) { //this particular horse is laying down -rotate and translate to correct position 
		model = rotate_z_deg(model, 100); 
		model = translate(model, vec3(horseLocation.v[0], -110.0f, horseLocation.v[2]));
	}
	else model = translate(model, horseLocation);//if standing simply translate to correct position 
	shader.setMat4("model", model);
	//texture mapping
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMaphorse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specMaphorse);


	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vpvbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vnvbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vtvbo);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, horseMesh.mPointCount);
}

