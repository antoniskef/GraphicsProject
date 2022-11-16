#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>

#include "piece.h"

using namespace std;

class Bishop: public Piece{
public:
    Bishop(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_BISHOP_H
