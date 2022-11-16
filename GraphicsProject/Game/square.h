#ifndef CHESS_SQUARE_H
#define CHESS_SQUARE_H

#include "GraphicsProject/Objects/objects.h"

class Square{
public:
    GLuint VAO;
    GLuint VerticiesVBO, NormalsVBO, UVVBO;
    std::vector<vec3> Vertices, Normals;
    std::vector<vec2> UVs;
    GLuint texture;
    GLuint textureSampler;
    mat4 MVP;
    mat4 Translation;

    void createSquare(int x_int, int z_int, Start* start);
    void squareLoop(Start* start);
    void squareFree();
};


#endif //CHESS_SQUARE_H
