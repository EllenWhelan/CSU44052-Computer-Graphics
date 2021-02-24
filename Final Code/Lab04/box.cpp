#include "box.h"
float vertices[] = {
    // positions          // normals           // texture coords
    -50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -50.0f,  50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -50.0f, -50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     50.0f,  50.0f, -50.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     50.0f, -50.0f,  50.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};
box::box()
{
}

box::box(vec3 boxloc)
{
    boxLocation = boxloc;
}

void box::genBox()
{
    Mesh mesh;
    boxMesh = mesh.load_mesh("meshes/horse.obj");

    glGenBuffers(1, &boxvpvbo);
    glBindBuffer(GL_ARRAY_BUFFER, boxvpvbo);
    glBufferData(GL_ARRAY_BUFFER, boxMesh.mPointCount * sizeof(vec3), &boxMesh.mVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &boxvnvbo);
    glBindBuffer(GL_ARRAY_BUFFER, boxvnvbo);
    glBufferData(GL_ARRAY_BUFFER, boxMesh.mPointCount * sizeof(vec3), &boxMesh.mNormals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &boxvtvbo);
    glBindBuffer(GL_ARRAY_BUFFER, boxvtvbo);
    glBufferData(GL_ARRAY_BUFFER, boxMesh.mPointCount * sizeof(vec2), &boxMesh.mTextureCoords[0], GL_STATIC_DRAW);

    glBindVertexArray(0); //release bidning 

}

void box::drawBox(Shader shader, int width, int height, unsigned int diffMap)
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
    model = translate(model, boxLocation);//translare to correct location
    shader.setMat4("model", model);
    //texture mapping
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffMap);

    glEnableVertexAttribArray(loc1);
    glBindBuffer(GL_ARRAY_BUFFER, boxvpvbo);
    glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc2);
    glBindBuffer(GL_ARRAY_BUFFER, boxvnvbo);
    glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc3);
    glBindBuffer(GL_ARRAY_BUFFER, boxvtvbo);
    glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // draw
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
    glDrawArrays(GL_TRIANGLES, 0, boxMesh.mPointCount);


}
