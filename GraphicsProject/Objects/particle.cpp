#include "particle.h"

Particle::Particle(Start* start){
    this->start = start;
}

void Particle::update(float deltaTime){

    position += velocity * deltaTime;
    if (position.y > 1) {
        color.a = color.a - (deltaTime * velocity.y / 4.0f);

    }

    if (position.y > 6){
        freeParticle();
    }

}

void Particle::create(){

    loadOBJWithTiny("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/sphere.obj", particleVertices, particleUVs, particleNormals);

    // VAO
    glGenVertexArrays(1, &particleVAO);
    glBindVertexArray(particleVAO);

    // vertex VBO
    glGenBuffers(1, &particleVerticiesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVerticiesVBO);
    glBufferData(GL_ARRAY_BUFFER, particleVertices.size() * sizeof(glm::vec3),
                 &particleVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    velocity.x = (double)(rand()) / ((double)(RAND_MAX/0.3f)) - 0.15;
    velocity.z = (double)(rand()) / ((double)(RAND_MAX/0.2f)) - 0.1;
    velocity.y = (double)(rand()) / ((double)(RAND_MAX/(2.5))) + 1.25;

    color = vec4(1,1,1,1);
    position = vec3(0,-3,0);

    scaleFactor = (double)(rand()) / ((double)(RAND_MAX/(0.1f))) + 0.2;

}

void Particle::draw(mat4 transfer) {

    glUseProgram(start->particleProgram);

    modelMatrix = transfer * translate(mat4(), position) *
                  scale(mat4(), vec3(scaleFactor, scaleFactor, scaleFactor));

    MVPMatrix = start->camera->projectionMatrix * start->camera->viewMatrix * modelMatrix;

    glUniformMatrix4fv(start->MVPLocation, 1, GL_FALSE, &MVPMatrix[0][0]);

    glUniform4f(start->colorLocation, color.r, color.g, color.b, color.a);

    glBindVertexArray(particleVAO);
    glDrawArrays(GL_TRIANGLES, 0, particleVertices.size());

}

void Particle::freeParticle() {
    glDeleteBuffers(1, &particleVerticiesVBO);
    glDeleteVertexArrays(1, &particleVAO);

}