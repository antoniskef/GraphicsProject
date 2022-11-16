#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>

#include "piece.h"

using namespace std;

class Queen: public Piece{
public:
    Queen(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_QUEEN_H
