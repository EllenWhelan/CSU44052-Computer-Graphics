#include "Spider.h"
Spider::Spider() {}

Spider::Spider(GLfloat x, GLfloat y, GLfloat z, bool forwardDir) {
	spiderX = x; 
	spiderY = y;
	spiderZ = z;
	forward = forwardDir;// whether spider is going backwards or forwards 
}
//function to load in spider mesh and create hierarchical object 
void Spider::genObject() {
	//filenames 
	const char* meshNames[25] = { "meshes/0.obj", "meshes/1.obj", "meshes/2.obj", "meshes/3.obj", "meshes/4.obj", "meshes/5.obj", "meshes/6.obj", "meshes/7.obj", "meshes/8.obj", "meshes/9.obj","meshes/10.obj",
	"meshes/11.obj", "meshes/12.obj", "meshes/13.obj", "meshes/14.obj", "meshes/15.obj", "meshes/16.obj", "meshes/17.obj", "meshes/18.obj", "meshes/19.obj","meshes/20.obj",
	"meshes/21.obj", "meshes/22.obj", "meshes/23.obj", "meshes/24.obj" };
	std::string matrixNames[25] = { "meshes/0.matrix", "meshes/1.matrix", "meshes/2.matrix", "meshes/3.matrix", "meshes/4.matrix", "meshes/5.matrix", "meshes/6.matrix", "meshes/7.matrix", "meshes/8.matrix", "meshes/9.matrix","meshes/10.matrix",
	"meshes/11.matrix", "meshes/12.matrix", "meshes/13.matrix", "meshes/14.matrix", "meshes/15.matrix", "meshes/16.matrix", "meshes/17.matrix", "meshes/18.matrix", "meshes/19.matrix","meshes/20.matrix",
	"meshes/21.matrix", "meshes/22.matrix", "meshes/23.matrix", "meshes/24.matrix" };

	//BODY -PARENT OF HIERARCHY
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
	Mesh mesh;
	mesh_data[0] = mesh.load_mesh(meshNames[0]);

	vp_vbo[0] = 0;
	glGenBuffers(1, &vp_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mVertices[0], GL_STATIC_DRAW);
	vn_vbo[0] = 0;
	glGenBuffers(1, &vn_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mNormals[0], GL_STATIC_DRAW);
	/*vt_vbo[0] = 0;
	glGenBuffers(1, &vt_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec2), &mesh_data[0].mTextureCoords[0], GL_STATIC_DRAW);*/

	//LEGS - CHILDREN OF HIERARCHY 
	for (int i = 1; i < TOTAL_PARTS; i++) {
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
		mesh_data[i] = mesh.load_mesh(meshNames[i]);

		vp_vbo[i] = 0;
		glGenBuffers(1, &vp_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mVertices[0], GL_STATIC_DRAW);
		vn_vbo[i] = 0;
		glGenBuffers(1, &vn_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mNormals[0], GL_STATIC_DRAW);
		/*vt_vbo[i] = 0;
		glGenBuffers(1, &vt_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec2), &mesh_data[i].mTextureCoords[0], GL_STATIC_DRAW);*/

	}
	glBindVertexArray(0); //release current binding 

};

//function to draw spider hierarchy 
void Spider::draw(Shader shader, int width, int height, unsigned int diffMap) { 
	shader.use();
	//locations in shader 
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;
	GLuint loc2 = 1;
	GLuint loc3 = 2; 

	// DRAW BODY -PARENT HIERARCHY  
	mat4 model = identity_mat4();
	model = scale(model, vec3(0.5f, 0.5f, 0.5f)); //scale to be smaller than chess pieces 
	if (!forward)model = rotate_y_deg(model, 180.0f); //face spider backwards if moving backwards 
	model = translate(model, vec3(spiderX, spiderY, spiderZ)); //move spider to location - spiderZ changes here for forwards and back wards movement
	shader.setMat4("model", model);
	//texture maps 
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
	/*glEnableVertexAttribArray(loc3);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[0]);
	glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
	// draw body 
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data[0].mPointCount);

	//DRAW LEGS -ITERATE THROUGH THEM 
	mat4 parentModel = model;
	int index = 3;
	//8 legs with 3 parts each is 24 
	while (index <= 24) {
		mat4 upperChild = identity_mat4();
		mat4 middleChild = identity_mat4();
		mat4 lowerChild = identity_mat4();

		//calculate rotations for animation 
		float upperChildRotation = 0.0f;
		float middleChildRotation = 0.0f;
		float lowerChildRotation = 0.0f; 
		float initialX = 0.0f; 
		if (index== 3 || index == 9 || index == 18 || index == 24) { //odd upper legs 
			upperChildRotation = initialX + upperAnimation;
			middleChildRotation = (-upperChildRotation / 3.0f) - middleAnimation;
			lowerChildRotation = lowerAnimation;
		}
		else if (index == 6 || index == 12 || index == 15 || index == 21) { //even upper legs 
			upperChildRotation = initialX - upperAnimation;
			middleChildRotation = (-upperChildRotation / 3.0f) - middleAnimation;
			lowerChildRotation = - lowerAnimation;
		}
		//rotations first - all parts start with joint in middle so rotate before translating them to proper location in hierarchy
		upperChild = rotate_x_deg(upperChild, upperChildRotation);
		middleChild= rotate_x_deg(middleChild,middleChildRotation);
		lowerChild= rotate_x_deg(lowerChild, lowerChildRotation);

		//translations
		//do upper translations 
		upperChild = translate(upperChild, trans[index]);//to proper location
		upperChild = parentModel * upperChild; //make upperleg child of body parent

		//do all middle child translations
		middleChild = translate(middleChild, trans[index - 1]-trans[index]);//to proper location
		middleChild = upperChild * middleChild; //make middle leg child of upper leg parent

		//do all lowerChild translations
		lowerChild = translate(lowerChild, trans[index - 2]-trans[index-1]);//to proper location
		lowerChild = middleChild * lowerChild; //make lower leg child of middle leg parent
				
		//draw upper leg		
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		/*glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
		// draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, upperChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index].mPointCount);

		//draw middle leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index-1]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index-1]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		/*glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-1]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
		//draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, middleChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index-1].mPointCount);
	
		//draw lower leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, vp_vbo[index-2]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, vn_vbo[index-2]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		/*glEnableVertexAttribArray(loc3);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-2]);
		glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/
		//draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lowerChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index-2].mPointCount);

		//reset 
		index = index + 3; //increase index to access next upper leg 
	}
	
}

//function to vary upper, middle and lower animation variables,to animate spider and handle movement through scene 
void Spider::updateSceneSpider(float delta, GLfloat currentSpeed, bool moving ) {
	//animation of spiders legs 
	float currentTime = timeGetTime();
	upperAnimation = cos(currentTime / 240.0f) * 30.0f;
	lowerAnimation = sin(currentTime / 240.0f) * 20.0f;
	middleAnimation = sin(currentTime / 240.0f) * 20.0f;

	//moving spiders forwards/backwards through scene 
	if (moving) {
		if (forward)spiderZ += currentSpeed;//0.0151f;
		else if (!forward)spiderZ -= currentSpeed;// 0.0151f;
	}

}


