#ifndef CHESS_KING_H
#define CHESS_KING_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>

#include "piece.h"

using namespace std;

class King: public Piece{
public:
    King(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_KING_H
