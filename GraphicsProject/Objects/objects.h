#ifndef CHESS_OBJECTS_H
#define CHESS_OBJECTS_H

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "board.h"
#include "piece.h"
#include "pawn.h"
#include "tower.h"
#include "horse.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "particleSystem.h"

using namespace std;

class Objects{
public:
    std::vector<Board> board;
    Piece* pieces[32];
    int occupied[32][2];
    int movePiece = 0;
    int eatPiece = 0;
    int x_start, z_start, x_end, z_end;
    float vx0, vz0, vy0;
    float wx, wz;
    int middle;
    int arrived;
    int fell;
    int number;
    int eatenPiece;
    int c=0;
    int cTwo=0;
    int fallPiece=-1;
    int fallPieceTwo=-1;
    int breaking;
    float time;
    Piece* brokenPiece;
    int created = 0;
    mat4 model;
    mat4 MVPMatrix;
    vec4 colorPearl = vec4(0.65, 0.53885, 0.53885, 0.922);
    vec4 colorObsidian = vec4(0.12685, 0.118, 0.15635, 0.82);
    GLfloat timeStartBreak;
    GLfloat timeBreak;
    int down;
    int endBreak;
    int eatenWhite = 0;
    int eatenBlack = 7;
    int c1=0;
    int c2=0;
    int c3=0;
    float k;
    ParticleSystem* particleSystem;
    int newParticleSystem = 1;
    int queen;
    float timeStartSmoke;
    mat4 transfer;

    struct Material {
        vec4 Ka;
        vec4 Kd;
        vec4 Ks;
        float Ns;
    };

    const Material obsidian{
            vec4{ 0.05375, 0.05, 0.06625, 1 },
            vec4{ 0.18275, 0.17, 0.22525, 1 },
            vec4{ 0.332741, 0.328634, 0.346435, 1 },
            38.4f
    };

    const Material pearl{
            vec4{ 0.25, 0.20725, 0.20725, 1 },
            vec4{ 1, 0.829, 0.829, 1 },
            vec4{ 0.296648, 0.296648, 0.296648, 1 },
            11.264f
    };


    Objects();
    void createAll();
    void modelAll();
    void loopAll();
    void freeAll();
    void occupyAll();
    void createObjects(Start* start);
    void uploadLight(Start* start);
    void uploadMaterial(const Material& mtl, Start* start);
    void depthLoopAll();
    void depthPass(mat4 viewMatrix, mat4 projectionMatrix, Start* start);
    void lightingPass(mat4 viewMatrix, mat4 projectionMatrix, Start* start);
    void lightLoopAll(Start* start);
    int whichPiece(int x_piece, int z_piece);
    void startMove(int x_piece, int z_piece, int x, int z);
    void startEat(int x_piece, int z_piece, int x, int z);
    void startMoveHorse(int x_piece, int z_piece, int x, int z);
    void startEatHorse(int x_piece, int z_piece, int x, int z) ;
    void middleMove();
    void middleEat();
    void middleMoveHorse();
    void endMove();
    void endMoveHorse();
    void arrival();
    void eatArrival();
    void arrivalHorse();
    void updatePosition();
    void startFall();
    void startFallHorse();
    void endFall();
    void finishMove();
    void finishEat();
    void breakPiece(Start* start);
    void startDown();
    void finishBreak();
    mat4 ifHorse(int number);
    void pawnToQueen(Start* start, float dt);
    void platforms(Drawable* plat, mat4 modelMatrix, Start* start);
};

#endif //CHESS_OBJECTS_H
