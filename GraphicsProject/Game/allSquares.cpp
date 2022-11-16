#include "allSquares.h"
#include <iostream>

#define W_WIDTH 1024
#define W_HEIGHT 768

AllSquares::AllSquares(Start* start, int* p){
    this->start = start;
    this->p = p;
}

void AllSquares::createAllSquares(){

    int j = 0;

    while (p[j] != -1) {
        size++;
        j = j + 2;
    }

    for(int i = 0; i < size; ++i) {
        square.push_back(Square());
    }

    int i = 0;
    int k = 0;
    while (p[i] != -1) {
        cout << p[i];
        cout << p[i + 1];
        square[k].createSquare(p[i], p[i + 1], start);
        i = i + 2;
        k++;
    }
}

void AllSquares::loopAllSquares(){

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glUseProgram(start->shaderProgram);

    glUniformMatrix4fv(start->viewMatrixLocation, 1, GL_FALSE, &start->camera->viewMatrix[0][0]);
    glUniformMatrix4fv(start->projectionMatrixLocation, 1, GL_FALSE, &start->camera->projectionMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, start->depthTexture);
    glUniform1i(start->depthMapSampler, 0);

    mat4 light_VP = start->light->lightVP();
    glUniformMatrix4fv(start->lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, start->textureSquare);
    glUniform1i(start->diffuseColorSampler, 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, start->modelSpecularTexture);
    glUniform1i(start->specularColorSampler, 2);
    glUniform1i(start->useTextureLocation, 1);
    for (int k = 0; k<size; k++) {
        square[k].squareLoop(start);
    }
}

void AllSquares::freeAllSquares(){

    for (int k = 0; k<size; k++) {
        square[k].squareFree();
    }
}

