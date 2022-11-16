#include "tower.h"

Tower::Tower(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void Tower::createBoundingBox(){
    rUp = 0.278081;
    yUp = 1.20187;
    rDown = 0.379308;
    yDown = 0.061447;

    rBase = 0.347541;
}