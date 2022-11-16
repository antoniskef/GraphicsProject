#include "piece.h"
#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Piece::Piece(Start* start, const char* path_object, const char* path_texture, int color, int number){
    this->start = start;
    this->path_object = path_object;
    this->path_texture = path_texture;
    this->color = color;
    this->number = number;
}

void Piece::createPiece(){

    loadOBJ(path_object, pieceVertices, pieceUVs, pieceNormals);

    // VAO
    glGenVertexArrays(1, &pieceVAO);
    glBindVertexArray(pieceVAO);

    // vertex VBO
    glGenBuffers(1, &pieceVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pieceVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, pieceVertices.size() * sizeof(glm::vec3),
                 &pieceVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // normals VBO
    glGenBuffers(1, &pieceNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pieceNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, pieceNormals.size() * sizeof(glm::vec3),
                 &pieceNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

}

void Piece::modelPiece(int pieceMove_x, int pieceMove_z){

    pieceRotation = ifHorse(number);
    pieceTranslation = glm::translate(glm::mat4(), glm::vec3(pieceMove_x, 0.1, pieceMove_z));

    pieceModel = pieceTranslation * pieceRotation;

    piecePosition_x = pieceMove_x;
    x.x = pieceMove_x;
    piecePosition_z = pieceMove_z;
    x.z = pieceMove_z;
}


void Piece::depthLoopPiece(){

    glUniformMatrix4fv(start->shadowModelLocation, 1, GL_FALSE, &pieceModel[0][0]);

    glBindVertexArray(pieceVAO);
    glDrawArrays(GL_TRIANGLES, 0, pieceVertices.size());
}

void Piece::lightLoopPiece(){

    glUniformMatrix4fv(start->modelMatrixLocation, 1, GL_FALSE, &pieceModel[0][0]);

    glBindVertexArray(pieceVAO);
    glDrawArrays(GL_TRIANGLES, 0, pieceVertices.size());
}

void Piece::freePiece(){
    glDeleteBuffers(1, &pieceVerticiesVBO);
    glDeleteBuffers(1, &pieceNormalsVBO);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &pieceVAO);
}

void Piece::update(float t, float dt) {

    advanceState(t, dt);

    mat4 translation = translate(mat4(), vec3(x.x, x.y, x.z));

#ifdef USE_QUATERNIONS
    mat4 rotation = mat4_cast(q);
#else
    mat4 rotation = mat4(R);
#endif

    // fall
    float k;
    float wx;
    float wz;
    if (w.x != 0 && w.z != 0){
        k = sqrt( (rBase * rBase) / 2.0f);
        wx = w.x / abs(w.x);
        wz = w.z / abs(w.z);
    }
    else if (w.x != 0){
        k = sqrt( (rBase * rBase) );
        wx = w.x / abs(w.x);
        wz = 0;
    }
    else if (w.z != 0){
        k = sqrt( (rBase * rBase) );
        wx = 0;
        wz = w.z / abs(w.z);
    }
    else{
        k = 0;
        wx = 0;
        wz = 0;
    }

    pieceModel = translation  * (translate(mat4(), vec3( -k*wz, 0, +k*wx))) * rotation *
            (translate(mat4(), vec3(+k*wz, 0, -k*wx))) * pieceRotation;

    if (wx == 0 && wz == 0 ){
        pieceModel = translation *  pieceRotation;
    }

}

void Piece::createBoundingBox() {}


mat4 Piece::ifHorse(int number){
    mat4 pieceRotation;
    if (number == 10 || number == 11){
        pieceRotation = glm::rotate(glm::mat4(), -3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (number == 26 || number == 27){
        pieceRotation = glm::rotate(glm::mat4(), 3.14f/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else{
        pieceRotation = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    return pieceRotation;
}

