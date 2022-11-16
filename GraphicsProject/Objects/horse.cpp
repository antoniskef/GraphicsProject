#include "horse.h"

Horse::Horse(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void Horse::createBoundingBox(){
    xUp = 0.417807;
    zUp = 0.137196;
    yUp = 0.873700;
    rDown = 0.377423;
    yDown = 0.062042;

    rBase = 0.358574;
}