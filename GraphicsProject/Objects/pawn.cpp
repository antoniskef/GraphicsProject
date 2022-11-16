#include "pawn.h"

Pawn::Pawn(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void Pawn::createBoundingBox(){
    rUp = 0.167376;
    yUp = 0.846734;
    rDown = 0.318754;
    yDown = 0.040663;

    rBase = 0.302878;
}


