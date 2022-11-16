#ifndef CHESS_HORSE_H
#define CHESS_HORSE_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>

#include "piece.h"

using namespace std;

class Horse: public Piece{
public:
    Horse(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_HORSE_H
