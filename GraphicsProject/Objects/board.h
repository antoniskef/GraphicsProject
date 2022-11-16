#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>
#include <glfw3.h>
#include <GraphicsProject/Game/start.h>

using namespace std;
using namespace glm;

class Board{
public:
    GLuint boardVAO;
    GLuint boardVerticiesVBO, boardUVVBO, boardNormalsVBO;
    std::vector<vec3> boardVertices, boardNormals;
    std::vector<vec2> boardUVs;
    GLuint texture;
    GLuint textureSampler;
    Start* start;

    mat4 boardScaling;
    mat4 boardRotation;
    mat4 boardTranslation;
    mat4 boardModel;
    mat4 boardMVP;

    int i;
    int j;

    Board(Start* start, int i, int j);
    void createBoard();
    void modelBoard();
    void loopBoard();
    void depthLoopBoard(mat4 modelMatrix);
    void lightLoopBoard(mat4 modelMatrix);
    void freeBoard();
};


#endif //CHESS_BOARD_H
