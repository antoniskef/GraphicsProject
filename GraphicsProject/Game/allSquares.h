#ifndef CHESS_ALLSQUARES_H
#define CHESS_ALLSQUARES_H

#include "square.h"

class AllSquares{
public:
    int size = 0;
    //Square *square;
    std::vector<Square> square;
    Start* start;
    int* p;

    AllSquares(Start* start, int* p);
    void createAllSquares();
    void loopAllSquares();
    void freeAllSquares();
};

#endif //CHESS_ALLSQUARES_H
