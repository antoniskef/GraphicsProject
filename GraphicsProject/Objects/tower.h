#ifndef CHESS_TOWER_H
#define CHESS_TOWER_H

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <common/camera.h>

#include "piece.h"

using namespace std;

class Tower: public Piece{
public:
    Tower(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_TOWER_H
