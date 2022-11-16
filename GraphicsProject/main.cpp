#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "GraphicsProject/Game/start.h"
#include "GraphicsProject/Game/play.h"
#include "GraphicsProject/Objects/objects.h"
#include "GraphicsProject/Game/allSquares.h"

using namespace std;
using namespace glm;

#define W_WIDTH 1024
#define W_HEIGHT 768

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
vec3 rayWorldSpace(double xPos, double yPos);
void mainLoop();

Start* start;
Objects* objects;
Play* play;
int* p;
AllSquares* allSquares = new AllSquares(start, p);
int x_piece, z_piece;
int endGame = 0;


int main(void) {

    start = new Start();
    objects = new Objects();
    play = new Play(start, objects);

    try {
        start->initialize();
        start->createContext();
        objects->createObjects(start);
        mainLoop();
        play->terminate();
    } catch (exception& ex) {
        cout << ex.what() << endl;
        getchar();
        play->terminate();
        return -1;
    }
    return 0;
}


void mainLoop() {
    objects->createAll();

    objects->modelAll();

    objects->occupyAll();

    int array[1] = {-1};
    p = array;

    float t = glfwGetTime();

    do {
        float currentTime = glfwGetTime();
        float dt = currentTime - t;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        start->light->update();
        mat4 light_proj = start->light->projectionMatrix;
        mat4 light_view = start->light->viewMatrix;

        objects->depthPass(light_view, light_proj, start);

        start->camera->update();
        mat4 projectionMatrix = start->camera->projectionMatrix;
        mat4 viewMatrix = start->camera->viewMatrix;

        glfwSetKeyCallback(start->window, pollKeyboard);

        objects->lightingPass(viewMatrix, projectionMatrix, start);

        if (objects->movePiece == 0 && objects->eatPiece == 0) {

            glfwSetMouseButtonCallback(start->window, mouse_button_callback);

            if (play->enable == 1) {
                if (play->first == 1) {
                    allSquares->freeAllSquares();
                    allSquares = new AllSquares(start, p);
                    allSquares->createAllSquares();
                    play->first = 0;
                }
                allSquares->loopAllSquares();
            }
        }

        else if (objects->movePiece == 1){
            objects->pieces[objects->number]->update(t, dt);
            objects->updatePosition();

            if (objects->middle == 0) objects->middleMove();
            if (objects->arrived == 0) objects->arrival();
            if (objects->arrived == 1) objects->pawnToQueen(start, dt);

            if (play->turn == 0) {
                if (objects->arrived == 1 && objects->queen == 1){
                    objects->movePiece = 0;
                    play->turn = 1;
                    start->camera->cameraMove = 1;
                }
            }
            else if (play->turn == 1) {
                if (objects->arrived == 1 && objects->queen == 1) {
                    objects->movePiece = 0;
                    play->turn = 0;
                    start->camera->cameraMove = 2;
                }
            }
        }

        else if (objects->eatPiece == 1){
            objects->pieces[objects->number]->update(t, dt);
            if (objects->c==1) objects->pieces[objects->fallPiece]->update(t, dt);
            if (objects->cTwo == 1) objects->pieces[objects->fallPieceTwo]->update(t, dt);
            objects->pieces[objects->eatenPiece]->update(t, dt);
            objects->updatePosition();

            if (objects->middle == 0) objects->middleEat();
            if (objects->arrived == 0) objects->eatArrival();
            if (objects->fell == 0) objects->endFall();
            if (objects->breaking == 1) objects->breakPiece(start);
            if (objects->down == 0) objects->finishBreak();
            if (objects->endBreak == 1) objects->pawnToQueen(start, dt);


            if (play->turn == 0) {
                if (objects->endBreak == 1 && objects->queen == 1){

                    if (endGame == 1) break;

                    play->turn = 1;
                    start->camera->cameraMove = 1;
                    objects->eatPiece = 0;
                }
            }
            else if (play->turn == 1) {
                if (objects->endBreak == 1 && objects->queen == 1) {

                    if (endGame == 1) break;

                    play->turn = 0;
                    start->camera->cameraMove = 2;
                    objects->eatPiece = 0;
                }
            }
        }

        glfwSwapBuffers(start->window);
        glfwPollEvents();

        t+=dt;

    } while (glfwGetKey(start->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(start->window) == 0);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        // ray in world space
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        vec3 ray_wor = rayWorldSpace(xPos, yPos);

        // board ray intersection
        vec3 boardNormal = vec3(0,1,0);
        vec3 p0 = vec3(3.5, 0.1, 3.5);

        float t = dot(p0 - start->camera->position, boardNormal) / dot(ray_wor, boardNormal);
        vec3 point = start->camera->position + ray_wor * t;

        float x = floor(point.x + 0.5);
        float z = floor(point.z + 0.5);

        int yes = 0;
        int i = 0;
        while (p[i] != -1) {
            if (p[i] == x && p[i+1] == z){
                yes = 1;
            }
            i = i + 2;
        }

        if (yes){

            objects->eatenPiece = -1;
            objects->number = objects->whichPiece(x_piece, z_piece);
            objects->eatenPiece = objects->whichPiece(x, z);
            if (objects->eatenPiece == 15 || objects->eatenPiece == 31) endGame = 1;

            if (objects->eatenPiece == -1) {
                objects->startMove(x_piece, z_piece, x, z);
                objects->movePiece = 1;
            }
            else if (objects->eatenPiece > -1){
                objects->startEat(x_piece, z_piece, x, z);
                objects->eatPiece = 1;
            }

            play->enable = 0;
            p[0] = -1;
        }
        else{
            if (x>-1 && x<8 && z>-1 && z<8) {
                p = play->selected(x, z);
                x_piece = x;
                z_piece = z;
            }
        }
    }
}

void pollKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        ofstream MyFile("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/save.txt");

        if (MyFile.is_open()) {
            for (int i = 0; i < 32; i++) {
                MyFile << objects->pieces[i]->number << endl;
                MyFile << objects->pieces[i]->piecePosition_x << endl;
                MyFile << objects->pieces[i]->piecePosition_z << endl;
            }
            MyFile << play->turn << endl;
            MyFile << objects->eatenWhite << endl;
            MyFile << objects->eatenBlack << endl;

            MyFile.close();
        }
        else {
            cout << "Cannot open file";
        }
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        ifstream MyFile("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/save.txt");

        if (MyFile.is_open()) {
            string s;
            int num;
            int *nums = (int *) malloc(99 * sizeof(int));
            int i = 0;
            while (getline(MyFile, s)) {
                num = stoi(s);
                nums[i] = num;
                i++;
            }
            int k = 0;
            for (int j = 0; j < 32; j++) {
                objects->pieces[j]->number = nums[k];
                objects->pieces[j]->piecePosition_x = nums[k + 1];
                objects->pieces[j]->piecePosition_z = nums[k + 2];
                objects->pieces[j]->x.x = nums[k+1];
                objects->pieces[j]->x.z = nums[k+2];
                objects->pieces[j]->pieceModel = translate(mat4(), vec3( nums[k + 1], 0.1,nums[k + 2])) *
                        objects->pieces[j]->pieceRotation;
                k = k + 3;

                objects->pieces[j]->q = quat(0, vec3(0, 0, 0));
            }
            objects->occupyAll();

            if (play->turn != nums[k]) {
                start->camera->cameraMove = 2 - nums[k];
            }

            play->turn = nums[k];

            objects->eatenWhite = nums[k+1];
            objects->eatenBlack = nums[k+2];

            MyFile.close();
            free(nums);
        }
        else {
            cout << "Cannot open file";
        }
    }
}

vec3 rayWorldSpace(double xPos, double yPos){
    float x = (2.0f * xPos) / W_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * yPos) / W_HEIGHT;
    float z = 1.0f;
    vec3 ray_nds = vec3(x, y, z);

    vec4 ray_clip = vec4(x, y, -1.0, 1.0);

    vec4 ray_eye = inverse(start->camera->projectionMatrix) * ray_clip;

    ray_eye = vec4(vec2(ray_eye), -1.0, 0.0);

    vec3 ray_wor = vec3((inverse(start->camera->viewMatrix) * ray_eye));

    ray_wor = normalize(ray_wor);

    return ray_wor;
}

