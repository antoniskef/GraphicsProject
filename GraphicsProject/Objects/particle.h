#ifndef CHESS_PARTICLE_H
#define CHESS_PARTICLE_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/model.h>
#include <stdlib.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GraphicsProject/Game/start.h"

using namespace std;
using namespace glm;

class Particle{
public:
    vec3 position;
    vec3 velocity;
    vec4 color;

    GLuint particleVAO;
    GLuint particleVerticiesVBO;
    std::vector<vec3> particleVertices, particleNormals;
    std::vector<vec2> particleUVs;

    Start* start;

    double scaleFactor;

    mat4 modelMatrix;
    mat4 MVPMatrix;

    Particle(Start* start);
    void create();
    void update(float deltaTime);
    void draw(mat4 transfer);
    void freeParticle();
};

#endif //CHESS_PARTICLE_H
