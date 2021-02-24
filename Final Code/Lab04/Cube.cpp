#include "cube.h"

Cube::Cube()
{
}

Cube::Cube(vec3 cubeloc)
{
    cubeLocation = cubeloc;
}

void Cube::gencube()
{
    Mesh mesh;
    cubeMesh = mesh.load_mesh("meshes/horse.obj");

    glGenBuffers(1, &cubevpvbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubevpvbo);
    glBufferData(GL_ARRAY_BUFFER, cubeMesh.mPointCount * sizeof(vec3), &cubeMesh.mVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &cubevnvbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubevnvbo);
    glBufferData(GL_ARRAY_BUFFER, cubeMesh.mPointCount * sizeof(vec3), &cubeMesh.mNormals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &cubevtvbo);
    glBindBuffer(GL_ARRAY_BUFFER, cubevtvbo);
    glBufferData(GL_ARRAY_BUFFER, cubeMesh.mPointCount * sizeof(vec2), &cubeMesh.mTextureCoords[0], GL_STATIC_DRAW);

    printf("Finished gen cube");
    glBindVertexArray(0); //release bidning 

}

void Cube::drawcube(Shader shader, int width, int height, unsigned int diffMap)
{
    shader.use();
    //loc in shader 
    int matrix_location = glGetUniformLocation(shader.ID, "model");
    GLuint loc1 = 0;
    GLuint loc2 = 1;
    GLuint loc3 = 2;

    mat4 model = identity_mat4();
    //model = scale(model, vec3(10.0f, 10.0f, 10.0f)); //scale to proper position 
    //model = rotate_x_deg(model, -90); //rotate to proper orientatoion
    model = translate(model, cubeLocation);//translare to correct location
    shader.setMat4("model", model);
    //texture mapping
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, diffMap);
    
    glEnableVertexAttribArray(loc1);
    glBindBuffer(GL_ARRAY_BUFFER, cubevpvbo);
    glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc2);
    glBindBuffer(GL_ARRAY_BUFFER, cubevnvbo);
    glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(loc3);
    glBindBuffer(GL_ARRAY_BUFFER, cubevtvbo);
    glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // draw
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
    glDrawArrays(GL_TRIANGLES, 0, cubeMesh.mPointCount);


}
