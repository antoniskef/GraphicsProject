#ifndef CHESS_PLAY_H
#define CHESS_PLAY_H

#include "GraphicsProject/Objects/objects.h"

class Play {
public:
    Start* start;
    Objects* objects;

    int enable = 0;
    int first = 1;
    int turn = 0;

    int *moves = (int*) malloc(55*sizeof(int));

    Play(Start* start, Objects* objects);
    int* selected(int x, int z);
    int* check(int occupied[32][2], int x, int z);
    int* possibleMovesPawn(Objects* objects, Piece* piece);
    int* possibleMovesTower(Objects* objects, Piece* piece);
    int* possibleMovesHorse(Objects* objects, Piece* piece);
    int* possibleMovesBishop(Objects* objects, Piece* piece);
    int* possibleMovesQueen(Objects* objects, Piece* piece);
    int* possibleMovesKing(Objects* objects, Piece* piece);
    int checkAround(Objects* objects, int x, int z, int color);
    void terminate();
};

#endif //CHESS_PLAY_H
