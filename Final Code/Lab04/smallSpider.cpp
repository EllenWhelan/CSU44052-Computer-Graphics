#include "smallSpider.h"

smallSpider::smallSpider() {}

smallSpider::smallSpider(GLfloat x, GLfloat y, GLfloat z, bool forwardDir, bool faceR ,GLfloat offset) {
	smallspiderX = x;
	smallspiderY = y;
	smallspiderZ = z;
	smallSpiderForward = forwardDir; //whether spider is going forwrads or backwrads 
	faceRight = faceR; //whether starts by turning right or not
	walkingDirOffset = offset; //offset by which spider starts off facing 
}

//function to load in mesh and matrix and create VAOs for smaller spiders 
void smallSpider::genSmallSpider() {
	//filenames 
	const char* meshNames[25] = { "meshes/0.obj", "meshes/1.obj", "meshes/2.obj", "meshes/3.obj", "meshes/4.obj", "meshes/5.obj", "meshes/6.obj", "meshes/7.obj", "meshes/8.obj", "meshes/9.obj","meshes/10.obj",
	"meshes/11.obj", "meshes/12.obj", "meshes/13.obj", "meshes/14.obj", "meshes/15.obj", "meshes/16.obj", "meshes/17.obj", "meshes/18.obj", "meshes/19.obj","meshes/20.obj",
	"meshes/21.obj", "meshes/22.obj", "meshes/23.obj", "meshes/24.obj" };
	std::string matrixNames[25] = { "meshes/0.matrix", "meshes/1.matrix", "meshes/2.matrix", "meshes/3.matrix", "meshes/4.matrix", "meshes/5.matrix", "meshes/6.matrix", "meshes/7.matrix", "meshes/8.matrix", "meshes/9.matrix","meshes/10.matrix",
	"meshes/11.matrix", "meshes/12.matrix", "meshes/13.matrix", "meshes/14.matrix", "meshes/15.matrix", "meshes/16.matrix", "meshes/17.matrix", "meshes/18.matrix", "meshes/19.matrix","meshes/20.matrix",
	"meshes/21.matrix", "meshes/22.matrix", "meshes/23.matrix", "meshes/24.matrix" };

	//LOAD IN BODY AND CREATE VAO	
	std::string matFile = matrixNames[0];
	std::ifstream Matfile(matFile);
	if (Matfile.good()) {
		std::string sLine;
		std::getline(Matfile, sLine);
		std::istringstream iss(sLine);
		float x, y, z;
		iss >> x >> y >> z;
		smallSpiderTrans[0] = vec3(-x, -z, y);
	}

	Mesh mesh;
	mesh_data[0] = mesh.load_mesh(meshNames[0]);

	smallvp_vbo[0] = 0;
	glGenBuffers(1, &smallvp_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mVertices[0], GL_STATIC_DRAW);
	smallvn_vbo[0] = 0;
	glGenBuffers(1, &smallvn_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec3), &mesh_data[0].mNormals[0], GL_STATIC_DRAW);
	/*vt_vbo[0] = 0;
	glGenBuffers(1, &vt_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh_data[0].mPointCount * sizeof(vec2), &mesh_data[0].mTextureCoords[0], GL_STATIC_DRAW);*/

	//LOAD IN LEGS 
	for (int i = 1; i < TOTAL_PARTS; i++) {
		std::string matFile = matrixNames[i];
		std::ifstream Matfile(matFile);
		if (Matfile.good()) {
			std::string sLine;
			std::getline(Matfile, sLine);
			std::istringstream iss(sLine);
			float x, y, z;
			iss >> x >> y >> z;
			smallSpiderTrans[i] = vec3(-x, -z, y);
		}
		mesh_data[i] = mesh.load_mesh(meshNames[i]);

		smallvp_vbo[i] = 0;
		glGenBuffers(1, &smallvp_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mVertices[0], GL_STATIC_DRAW);
		smallvn_vbo[i] = 0;
		glGenBuffers(1, &smallvn_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec3), &mesh_data[i].mNormals[0], GL_STATIC_DRAW);
		/*vt_vbo[i] = 0;
		glGenBuffers(1, &vt_vbo[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, mesh_data[i].mPointCount * sizeof(vec2), &mesh_data[i].mTextureCoords[0], GL_STATIC_DRAW);*/
	}
	glBindVertexArray(0); //releasing bound vao 
};


//function to draw spider
void smallSpider::draw(Shader shader, int width, int height, unsigned int diffMap) {

	shader.use();
	//loc in shader 
	int matrix_location = glGetUniformLocation(shader.ID, "model");
	GLuint loc1 = 0;
	GLuint loc2 = 1;
	GLuint loc3 = 2;

	// DRAW THE BODY 
	mat4 model = identity_mat4();
	model = scale(model, vec3(0.1f, 0.1f, 0.1f)); //scale to be smaller 
	if (!smallSpiderForward)model = rotate_y_deg(model, 180.0f); //face spider backwards if moving backwards 
	model = rotate_y_deg(model, walkingDirOffset); //use offset to rotate spider in the direction their facing 
	model = translate(model, vec3(smallspiderX, smallspiderY, smallspiderZ)); //move spider to location - spiderZ changes here for forwards and back wards movement
	shader.setMat4("model", model);
	//texture mapping
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, diffMap);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[0]);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[0]);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//glEnableVertexAttribArray(loc3);
	//glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[0]);
	//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	// draw
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data[0].mPointCount);

	//DRAW LEGS 
	mat4 parentModel = model;
	int index = 3;
	while (index <= 24) { //8 legs , 3 parts each 
		mat4 upperChild = identity_mat4();
		mat4 middleChild = identity_mat4();
		mat4 lowerChild = identity_mat4();

		//calculate rotations for animation 
		float upperChildRotation = 0.0f;
		float middleChildRotation = 0.0f;
		float lowerChildRotation = 0.0f;
		float initialX = 0.0f;
		if (index == 3 || index == 9 || index == 18 || index == 24) {//even upper legs 
			upperChildRotation = initialX + smallSpiUpperAnimation;
			middleChildRotation = (-upperChildRotation / 3.0f) - smallSpiMiddleAnimation;
			lowerChildRotation = smallSpiLowerAnimation;
		}
		else if (index == 6 || index == 12 || index == 15 || index == 21) {//odd upper legs 
			upperChildRotation = initialX - smallSpiUpperAnimation;
			middleChildRotation = (-upperChildRotation / 3.0f) - smallSpiMiddleAnimation;
			lowerChildRotation = -smallSpiLowerAnimation;
		}
		//rotations first - all parts start with joint in middle so must rotate before translating to proper pos in hierarchy 
		upperChild = rotate_x_deg(upperChild, upperChildRotation);
		middleChild = rotate_x_deg(middleChild, middleChildRotation);
		lowerChild = rotate_x_deg(lowerChild, lowerChildRotation);
		//translations
		//do upper translations 
		upperChild = translate(upperChild, smallSpiderTrans[index]);//to proper location
		upperChild = parentModel * upperChild;

		//do all middle child translations
		middleChild = translate(middleChild, smallSpiderTrans[index - 1] - smallSpiderTrans[index]);//to proper location - must get rid of previous parent translation
		middleChild = upperChild * middleChild;

		//do all lowerChild translations
		lowerChild = translate(lowerChild, smallSpiderTrans[index - 2] - smallSpiderTrans[index - 1]);//to proper location- must get rid of previous parent translation
		lowerChild = middleChild * lowerChild;


		//draw upper leg		
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[index]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[index]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//glEnableVertexAttribArray(loc3);
		//glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index]);
		//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, upperChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index].mPointCount);

		//draw middle leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[index - 1]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[index - 1]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//glEnableVertexAttribArray(loc3);
		//glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-1]);
		//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, middleChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index - 1].mPointCount);

		//draw lower leg 
		glEnableVertexAttribArray(loc1);
		glBindBuffer(GL_ARRAY_BUFFER, smallvp_vbo[index - 2]);
		glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(loc2);
		glBindBuffer(GL_ARRAY_BUFFER, smallvn_vbo[index - 2]);
		glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		//glEnableVertexAttribArray(loc3);
		//glBindBuffer(GL_ARRAY_BUFFER, vt_vbo[index-2]);
		//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//draw
		glUniformMatrix4fv(matrix_location, 1, GL_FALSE, lowerChild.m);
		glDrawArrays(GL_TRIANGLES, 0, mesh_data[index - 2].mPointCount);

		//reset 
		index = index + 3; //increase index to access next upper leg 
	}

}

//function to handle animation variables and movement direction
void smallSpider::updateSceneSmallSpider(float delta, bool moving ) {
	//animation of spiders legs 
	GLfloat speed = 40.0f;
	float currentTime = timeGetTime();
	smallSpiUpperAnimation = cos(currentTime / 240.0f) * speed; 
	smallSpiLowerAnimation = sin(currentTime / 240.0f) * (speed); 
	smallSpiMiddleAnimation = sin(currentTime / 240.0f) * (speed); 

	//moving spiders forwards/backwards through scene 
	if (moving) {
		if (smallSpiderForward)smallspiderZ += 0.051f;
		else if (!smallSpiderForward)smallspiderZ -= 0.051f;


		//changing direction spider faces in 
		if (faceRight) { //turn right until offset is at 35 degrees then switch directions 
			if (walkingDirOffset < 35) {
				walkingDirOffset += 0.5;
			}
			else {
				faceRight = false;
			}
		}
		else if (!faceRight) {//turn left until offset is -35 degrees then switch directions 
			if (walkingDirOffset > -35)walkingDirOffset -= 0.5;
			else faceRight = true;
		}
	}
	

}