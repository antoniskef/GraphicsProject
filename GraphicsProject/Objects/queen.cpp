#include "queen.h"

Queen::Queen(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void Queen::createBoundingBox(){
    rUp = 0.250573;
    yUp = 1.62158;
    rDown = 0.444136;
    yDown = 0.059148;

    rBase = 0.395382;
}
