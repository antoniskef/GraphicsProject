#include "square.h"
#include <common/texture.h>
#include <common/model.h>
#include <glm/gtc/matrix_transform.hpp>


void Square::createSquare(int x_int, int z_int, Start* start){

    float x = (float) x_int;
    float z = (float) z_int;
    Translation = glm::translate(glm::mat4(), glm::vec3(x, 0.1008, z));

    loadOBJ("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/square.obj", Vertices, UVs, Normals);

    //  VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // vertex VBO
    glGenBuffers(1, &VerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, VerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3),
                 &Vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &NormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, NormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3),
                 &Normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);


    // uvs VBO
    glGenBuffers(1, &UVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, UVVBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2),
                 &UVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Square::squareLoop(Start* start){

    glBindVertexArray(VAO);

    glUniformMatrix4fv(start->modelMatrixLocation, 1, GL_FALSE, &Translation[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, Vertices.size());

}

void Square::squareFree(){
    glDeleteBuffers(1, &VerticiesVBO);
    glDeleteBuffers(1, &UVVBO);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
}