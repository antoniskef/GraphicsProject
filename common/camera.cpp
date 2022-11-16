#include <glfw3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

using namespace std;
using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window) {
    position = vec3(-3, 10, 3.5);
    up = vec3(1, 0, 0);
    center = vec3(3.5, 0, 3.5);
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
    FoV = 45.0f;
    speed = 3.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
    cameraAngle = (3.0f*3.14f)/2.0f;
}

void Camera::update() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);



    if (cameraMove == 0) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if (position.y > 1) {
                position -= vec3(0, 1, 0) * deltaTime * speed;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if (position.y < 25) {
                position += vec3(0, 1, 0) * deltaTime * speed;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if (position.x < 25) {
                position += vec3(1, 0, 0) * deltaTime * speed;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if (position.x > -18) {
                position -= vec3(1, 0, 0) * deltaTime * speed;
            }
        }
    }
    else if (cameraMove == 1){
        if (cameraAngle>3.14f/2.0f) {
            cameraAngle -= (0.008) * 3.14 * deltaTime * 70;
            up = vec3(0, 1, 0);
            position = vec3(6.5 * sin(cameraAngle) + 3.5, 10, 6.5 * cos(cameraAngle) + 3.5);
        }
        else{
            cameraMove = 0;
            cameraAngle = 3.14f/2.0f;
            position = vec3(10, 10, 3.5);
            up = vec3(-1, 0, 0);
        }
    }
    else if (cameraMove == 2){
        if (cameraAngle>-3.14f/2.0f) {
            cameraAngle -= (0.008) * 3.14 * deltaTime * 70;
            up = vec3(0, 1, 0);
            position = vec3(6.5 * sin(cameraAngle) + 3.5, 10, 6.5 * cos(cameraAngle) + 3.5);
        }
        else{
            cameraMove = 0;
            cameraAngle = (3.0f*3.14f)/2.0f;
            position = vec3(-3, 10, 3.5);
            up = vec3(1, 0, 0);
        }
    }


    // Task 5.2: update view matrix so it always looks at the origin
    projectionMatrix = perspective(radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        center,
        up
    );


    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}