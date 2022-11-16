#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>
#include <common/RigidBody.h>

#include "GraphicsProject/Game/start.h"

using namespace std;
using namespace glm;

class Piece: public RigidBody {
public:
    Start* start;

    GLuint pieceVAO;
    GLuint pieceVerticiesVBO, pieceNormalsVBO;
    std::vector<vec3> pieceVertices, pieceNormals;
    std::vector<vec2> pieceUVs;
    GLuint texture;
    GLuint textureSampler;

    mat4 pieceScaling;
    mat4 pieceRotation;
    mat4 pieceTranslation;
    mat4 pieceModel;
    mat4 pieceMVP;

    float piecePosition_x;
    float piecePosition_z;
    int color;
    int number;

    float rUp, yUp, rDown, yDown, rBase;
    float xUp, zUp;

    const char* path_object;
    const char* path_texture;

    Piece(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createPiece();
    void modelPiece(int pieceMove_x, int pieceMove_z);
    void loopPiece();
    void freePiece();
    void depthLoopPiece();
    void lightLoopPiece();
    void update(float t, float dt);
    mat4 ifHorse(int number);
    virtual void createBoundingBox();
    //void breakPiece();
};

#endif //CHESS_PIECE_H
