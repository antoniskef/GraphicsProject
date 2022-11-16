#include "king.h"

King::King(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void King::createBoundingBox(){
    rUp = 0.253381;
    yUp = 1.55549;
    rDown = 0.47583;
    yDown = 0.054607;

    rBase = 0.43412;
}


