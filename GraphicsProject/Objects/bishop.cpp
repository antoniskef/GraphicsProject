#include "bishop.h"

Bishop::Bishop(Start* start, const char* path_object, const char* path_texture, int color, int number)
        : Piece(start, path_object, path_texture, color, number) {
}

void Bishop::createBoundingBox(){
    rUp = 0.211526;
    yUp = 1.20713;
    rDown = 0.370805;
    yDown = 0.052965;

    rBase = 0.341272;
}
