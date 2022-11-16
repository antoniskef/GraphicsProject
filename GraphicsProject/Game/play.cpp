#include "play.h"
#include <iostream>

Play::Play(Start* start, Objects* objects){
    this->objects = objects;
    this->start = start;
}

int* Play::selected(int x, int z){
    int* c;
    int* p;

    c = check(objects->occupied, x, z);

    if (c[0]==1 && ( (objects->pieces[c[1]]->color == 0 && turn == 0) || (objects->pieces[c[1]]->color == 1 && turn == 1) )){

        enable = 1;
        first = 1;
        if ((objects->pieces[c[1]]->number>-1  && objects->pieces[c[1]]->number<8) ||
            (objects->pieces[c[1]]->number>15  && objects->pieces[c[1]]->number<24)){
            p = possibleMovesPawn(objects, objects->pieces[c[1]]);
        }
        else if( (objects->pieces[c[1]]->number == 8)  || (objects->pieces[c[1]]->number == 9) ||
                (objects->pieces[c[1]]->number == 24)  || (objects->pieces[c[1]]->number == 25)){
            p = possibleMovesTower(objects, objects->pieces[c[1]]);
        }
        else if( (objects->pieces[c[1]]->number == 10)  || (objects->pieces[c[1]]->number == 11) ||
                 (objects->pieces[c[1]]->number == 26)  || (objects->pieces[c[1]]->number == 27)){
            p = possibleMovesHorse(objects, objects->pieces[c[1]]);
        }
        else if( (objects->pieces[c[1]]->number == 12)  || (objects->pieces[c[1]]->number == 13) ||
                 (objects->pieces[c[1]]->number == 28)  || (objects->pieces[c[1]]->number == 29)){
            p = possibleMovesBishop(objects, objects->pieces[c[1]]);
        }
        else if( (objects->pieces[c[1]]->number == 14)  || (objects->pieces[c[1]]->number == 30) ){
            p = possibleMovesQueen(objects, objects->pieces[c[1]]);
        }
        else{
            p = possibleMovesKing(objects, objects->pieces[c[1]]);
        }
        return p;
    }
    else{
        enable = 0;
        moves[0] = -1;
        p = moves;
        return p;
    }
}

int* Play::possibleMovesPawn(Objects* objects, Piece* piece) {

    free(moves);
    moves = (int*) malloc(9*sizeof(int));
    int k = 0;

    if (piece->color == 0 && turn == 0) {
        int c1 = 0;
        int c2 = 0;
        int c3 = 0;
        int c4 = 0;

        c1 = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z, piece->color);
        c2 = checkAround(objects, piece->piecePosition_x + 2, piece->piecePosition_z, piece->color);
        c3 = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z + 1, piece->color);
        c4 = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z - 1, piece->color);

        if (piece->piecePosition_x != 7) {
            // move
            if (c1==0) {
                moves[k] = piece->piecePosition_x + 1;
                moves[k+1] = piece->piecePosition_z;
                k = k + 2;

                if (piece->piecePosition_x == 1 && piece->color == 0 && c2==0) {
                    moves[k] = piece->piecePosition_x + 2;
                    moves[k+1] = piece->piecePosition_z;
                    k = k + 2;
                }
            }
            // eat
            if(c3==2){
                moves[k] = piece->piecePosition_x + 1;
                moves[k+1] = piece->piecePosition_z + 1;
                k = k + 2;
            }
            if(c4==2){
                moves[k] = piece->piecePosition_x + 1;
                moves[k+1] = piece->piecePosition_z - 1;
                k = k + 2;
            }
        }
    }

    if (piece->color == 1 && turn == 1) {

        int c1 = 0;
        int c2 = 0;
        int c3 = 0;
        int c4 = 0;

        c1 = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z, piece->color);
        c2 = checkAround(objects, piece->piecePosition_x - 2, piece->piecePosition_z, piece->color);
        c3 = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z + 1, piece->color);
        c4 = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z - 1, piece->color);

        if (piece->piecePosition_x != 0) {
            // move
            if (c1==0) {
                moves[k] = piece->piecePosition_x - 1;
                moves[k+1] = piece->piecePosition_z;
                k = k + 2;

                if (piece->piecePosition_x == 6 && piece->color == 1 && c2==0) {
                    moves[k] = piece->piecePosition_x - 2;
                    moves[k+1] = piece->piecePosition_z;
                    k = k + 2;
                }
            }
            // eat
            if(c3==2){
                moves[k] = piece->piecePosition_x - 1;
                moves[k+1] = piece->piecePosition_z + 1;
                k = k + 2;
            }
            if(c4==2){
                moves[k] = piece->piecePosition_x - 1;
                moves[k+1] = piece->piecePosition_z - 1;
                k = k + 2;
            }
        }
    }

    moves[k] = -1;

    return moves;
}


int* Play::possibleMovesTower(Objects* objects, Piece* piece){

    free(moves);
    moves = (int*) malloc(29*sizeof(int));
    int k = 0;

    int i1 = 1;
    while (true) {
        if ((i1 + piece->piecePosition_x) < 8) {
            int c = checkAround(objects, piece->piecePosition_x + i1, piece->piecePosition_z, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x + i1;
                    moves[k + 1] = piece->piecePosition_z;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x + i1;
                moves[k+1] = piece->piecePosition_z;
                k = k + 2;
                i1++;
            }
        }
        else{
            break;
        }
    }

    int i2 = 1;
    while (true) {
        if ((piece->piecePosition_x - i2) > -1) {
            int c = checkAround(objects, piece->piecePosition_x - i2, piece->piecePosition_z, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x - i2;
                    moves[k + 1] = piece->piecePosition_z;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x - i2;
                moves[k+1] = piece->piecePosition_z;
                k = k + 2;
                i2++;
            }
        }
        else{
            break;
        }
    }

    int i3 = 1;
    while (true) {
        if ((i3 + piece->piecePosition_z) < 8) {
            int c = checkAround(objects, piece->piecePosition_x, piece->piecePosition_z + i3, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x;
                    moves[k + 1] = piece->piecePosition_z + i3;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x;
                moves[k+1] = piece->piecePosition_z + i3;
                k = k + 2;
                i3++;
            }
        }
        else{
            break;
        }
    }

    int i4 = 1;
    while (true) {
        if ((piece->piecePosition_z - i4) > -1) {
            int c = checkAround(objects, piece->piecePosition_x, piece->piecePosition_z - i4, piece->color);
            if ( c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x;
                    moves[k + 1] = piece->piecePosition_z - i4;
                    k = k + 2;
                }
                moves[k] = -1;
                break;
            }
            else{
                moves[k] = piece->piecePosition_x;
                moves[k+1] = piece->piecePosition_z - i4;
                k = k + 2;
                i4++;
            }
        }
        else{
            moves[k] = -1;
            break;
        }
    }
    return moves;
}


int* Play::possibleMovesHorse(Objects* objects, Piece* piece) {
    free(moves);
    moves = (int *) malloc(17 * sizeof(int));
    int k = 0;
    // up right
    if ((2 + piece->piecePosition_x) < 8 && (1 + piece->piecePosition_z) < 8) {
        int c = checkAround(objects, piece->piecePosition_x + 2, piece->piecePosition_z + 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 2;
            moves[k + 1] = piece->piecePosition_z + 1;
            k = k + 2;
        }
    }
    // right up
    if ((1 + piece->piecePosition_x) < 8 && (2 + piece->piecePosition_z) < 8) {
        int c = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z + 2, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 1;
            moves[k + 1] = piece->piecePosition_z + 2;
            k = k + 2;
        }
    }
    // up left
    if ((2 + piece->piecePosition_x) < 8 && (piece->piecePosition_z - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x + 2, piece->piecePosition_z - 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 2;
            moves[k + 1] = piece->piecePosition_z - 1;
            k = k + 2;
        }
    }
    // left up
    if ((1 + piece->piecePosition_x) < 8 && (piece->piecePosition_z - 2) > -1) {
        int c = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z - 2, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 1;
            moves[k + 1] = piece->piecePosition_z - 2;
            k = k + 2;
        }
    }
    // down right
    if ((piece->piecePosition_x - 2) > -1 && (piece->piecePosition_z + 1) < 8) {
        int c = checkAround(objects, piece->piecePosition_x - 2, piece->piecePosition_z + 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 2;
            moves[k + 1] = piece->piecePosition_z + 1;
            k = k + 2;
        }
    }
    // right down
    if ((piece->piecePosition_x - 1) > -1 && (piece->piecePosition_z + 2) < 8) {
        int c = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z + 2, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 1;
            moves[k + 1] = piece->piecePosition_z + 2;
            k = k + 2;
        }
    }
    // down left
    if ((piece->piecePosition_x - 2) > -1 && (piece->piecePosition_z - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x - 2, piece->piecePosition_z - 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 2;
            moves[k + 1] = piece->piecePosition_z - 1;
            k = k + 2;
        }
    }
    // left down
    if ((piece->piecePosition_x - 1) > -1 && (piece->piecePosition_z - 2) > -1) {
        int c = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z - 2, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 1;
            moves[k + 1] = piece->piecePosition_z - 2;
            k = k + 2;
        }
    }

    moves[k] = -1;
    return moves;
}


int* Play::possibleMovesBishop(Objects* objects, Piece* piece){

    free(moves);
    moves = (int*) malloc(27*sizeof(int));
    int k = 0;

    // up right
    int i1 = 1;
    while (true) {
        if ( (i1 + piece->piecePosition_x) < 8 && (i1 + piece->piecePosition_z) < 8 ) {
            int c = checkAround(objects, piece->piecePosition_x + i1, piece->piecePosition_z + i1, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x + i1;
                    moves[k + 1] = piece->piecePosition_z + i1;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x + i1;
                moves[k+1] = piece->piecePosition_z + i1;
                k = k + 2;
                i1++;
            }
        }
        else{
            break;
        }
    }

    // up left
    int i2 = 1;
    while (true) {
        if ( (i2 + piece->piecePosition_x) < 8 && (piece->piecePosition_z - i2) > -1 ) {
            int c = checkAround(objects, piece->piecePosition_x + i2, piece->piecePosition_z - i2, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x + i2;
                    moves[k + 1] = piece->piecePosition_z - i2;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x + i2;
                moves[k+1] = piece->piecePosition_z - i2;
                k = k + 2;
                i2++;
            }
        }
        else{
            break;
        }
    }

    // down right
    int i3 = 1;
    while (true) {
        if ( (i3 + piece->piecePosition_z) < 8 && (piece->piecePosition_x - i3) > -1 ) {
            int c = checkAround(objects, piece->piecePosition_x - i3, piece->piecePosition_z + i3, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x - i3;
                    moves[k + 1] = piece->piecePosition_z + i3;
                    k = k + 2;
                }
                break;
            }
            else{
                moves[k] = piece->piecePosition_x - i3;
                moves[k+1] = piece->piecePosition_z + i3;
                k = k + 2;
                i3++;
            }
        }
        else{
            break;
        }
    }

    // down left
    int i4 = 1;
    while (true) {
        if ( (piece->piecePosition_x - i4) > -1 && (piece->piecePosition_z - i4) > -1 ) {
            int c = checkAround(objects, piece->piecePosition_x - i4, piece->piecePosition_z - i4, piece->color);
            if (c > 0) {
                if ( c == 2 ) {
                    moves[k] = piece->piecePosition_x - i4;
                    moves[k + 1] = piece->piecePosition_z - i4;
                    k = k + 2;
                }
                moves[k] = -1;
                break;
            }
            else{
                moves[k] = piece->piecePosition_x - i4;
                moves[k+1] = piece->piecePosition_z - i4;
                k = k + 2;
                i4++;
            }
        }
        else{
            moves[k] = -1;
            break;
        }
    }
    return moves;
}


int* Play::possibleMovesQueen(Objects* objects, Piece* piece){

    int* moves1 = (int*) malloc(29*sizeof(int));
    int* moves2 = (int*) malloc(27*sizeof(int));
    int* moves3 = (int*) malloc(55*sizeof(int));

    moves1 = possibleMovesTower(objects, piece);
    int i = 0;
    while (moves1[i] != -1){
        moves3[i] = moves1[i];
        i++;
    }

    moves2 = possibleMovesBishop(objects, piece);
    int j = 0;
    while (moves2[j] != -1){
        moves3[i] = moves2[j];
        i++;
        j++;
    }
    free(moves2);

    moves = moves3;
    moves[i] = -1;

    return moves;
}


int* Play::possibleMovesKing(Objects* objects, Piece* piece){
    free(moves);
    moves = (int *) malloc(17 * sizeof(int));
    int k = 0;

    // up
    if ((piece->piecePosition_x + 1) < 8) {
        int c = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 1;
            moves[k + 1] = piece->piecePosition_z;
            k = k + 2;
        }
    }
    // up right
    if ((piece->piecePosition_x + 1) < 8 && (piece->piecePosition_z + 1) < 8) {
        int c = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z + 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 1;
            moves[k + 1] = piece->piecePosition_z + 1;
            k = k + 2;
        }
    }
    // up left
    if ((piece->piecePosition_x + 1) < 8 && (piece->piecePosition_z - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x + 1, piece->piecePosition_z - 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x + 1;
            moves[k + 1] = piece->piecePosition_z - 1;
            k = k + 2;
        }
    }
    // right
    if ((piece->piecePosition_z + 1) < 8) {
        int c = checkAround(objects, piece->piecePosition_x, piece->piecePosition_z + 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x;
            moves[k + 1] = piece->piecePosition_z + 1;
            k = k + 2;
        }
    }
    // left
    if ((piece->piecePosition_z - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x, piece->piecePosition_z - 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x;
            moves[k + 1] = piece->piecePosition_z - 1;
            k = k + 2;
        }
    }
    // down left
    if ((piece->piecePosition_x - 1) > -1 && (piece->piecePosition_z - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z - 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 1;
            moves[k + 1] = piece->piecePosition_z - 1;
            k = k + 2;
        }
    }
    // down right
    if ((piece->piecePosition_x - 1) > -1 && (piece->piecePosition_z + 1) < 8) {
        int c = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z + 1, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 1;
            moves[k + 1] = piece->piecePosition_z + 1;
            k = k + 2;
        }
    }
    // down
    if ((piece->piecePosition_x - 1) > -1) {
        int c = checkAround(objects, piece->piecePosition_x - 1, piece->piecePosition_z, piece->color);
        if (c != 1) {
            moves[k] = piece->piecePosition_x - 1;
            moves[k + 1] = piece->piecePosition_z;
            k = k + 2;
        }
    }
    moves[k] = -1;
    return moves;
}


int* Play::check(int occupied[32][2], int x, int z){
    int* c;
    c[0] = 0;
    for (int i=0; i<32; i++){
        if (occupied[i][0] == x){
            if (occupied[i][1] == z) {
                c[0] = 1;
                c[1] = i;
            }
        }
    }
    return c;
}

int Play::checkAround(Objects* objects, int x, int z, int color){
    int c = 0;
    for (int i=0; i<32; i++){
        if (objects->occupied[i][0] == x){
            if (objects->occupied[i][1] == z) {
                if (objects->pieces[i]->color == color){
                    c = 1;
                }
                else{
                    c = 2;
                }
            }
        }
    }
    return c;
}

void Play::terminate() {
//    board->freeBoard();

    objects->freeAll();

    glDeleteProgram(start->shaderProgram);
    glDeleteProgram(start->depthProgram);
    glfwTerminate();
}

