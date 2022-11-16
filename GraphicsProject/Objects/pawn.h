#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "piece.h"

using namespace std;

class Pawn: public Piece{
public:
    Pawn(Start* start, const char* path_object, const char* path_texture, int color, int number);
    void createBoundingBox();
};

#endif //CHESS_PAWN_H
