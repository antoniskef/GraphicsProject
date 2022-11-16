#include "board.h"
#include <common/model.h>
#include <common/texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Board::Board(Start* start, int i, int j){
    this->start = start;
    this->i = i;
    this->j = j;
}

void Board::createBoard(){
    loadOBJ("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/squareBoard.obj", boardVertices, boardUVs, boardNormals);

    // VAO
    glGenVertexArrays(1, &boardVAO);
    glBindVertexArray(boardVAO);

    // vertex VBO
    glGenBuffers(1, &boardVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, boardVertices.size() * sizeof(glm::vec3),
                 &boardVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &boardNormalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardNormalsVBO);
    glBufferData(GL_ARRAY_BUFFER, boardNormals.size() * sizeof(glm::vec3),
                 &boardNormals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &boardUVVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boardUVVBO);
    glBufferData(GL_ARRAY_BUFFER, boardUVs.size() * sizeof(glm::vec2),
                 &boardUVs[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Board::modelBoard(){
    boardScaling = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
    boardRotation = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    boardTranslation = glm::translate(glm::mat4(), glm::vec3(i, 0, j));

    boardModel = boardRotation * boardTranslation * boardScaling;
}

//void Board::loopBoard() {
//    glUseProgram(start->shaderProgram);
//
//    glBindVertexArray(boardVAO);
//
//    boardMVP = start->camera->projectionMatrix * start->camera->viewMatrix * boardModel;
//
//    glUniformMatrix4fv(start->MVPLocation, 1, GL_FALSE, &boardMVP[0][0]);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glUniform1i(textureSampler, 0);
//
//    glDrawArrays(GL_TRIANGLES, 0, boardVertices.size());
//}

void Board::depthLoopBoard(mat4 modelMatrix){

    glUniformMatrix4fv(start->shadowModelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindVertexArray(boardVAO);
    glDrawArrays(GL_TRIANGLES, 0, boardVertices.size());
}

void Board::lightLoopBoard(mat4 modelMatrix){

    glUniformMatrix4fv(start->modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindVertexArray(boardVAO);
    glDrawArrays(GL_TRIANGLES, 0, boardVertices.size());
}

void Board::freeBoard(){
    glDeleteBuffers(1, &boardVerticiesVBO);
    glDeleteBuffers(1, &boardUVVBO);
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &boardVAO);
}