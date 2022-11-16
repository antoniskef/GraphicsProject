#include "objects.h"
#include "piece.h"

#define W_WIDTH 1024
#define W_HEIGHT 768
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

using namespace std;

Objects::Objects(){}

void Objects::createAll(){
    int k = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if ((i + j)%2 == 0) {
                board[k].createBoard();
            }
            else{
                board[k].createBoard();
            }
            k++;
        }
    }

    for (int i = 0; i<32; i++){
         pieces[i]->createPiece();
         pieces[i]->createBoundingBox();
         pieces[i]->x.y = 0.1;
    }
}

void Objects::modelAll() {

    int k = 0;
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[k].modelBoard();
            k++;
        }
    }

    for (int i = 0; i < 8; i++) {
        pieces[i]->modelPiece(1, i);
    }
    for (int i = 16; i < 24; i++) {
        pieces[i]->modelPiece(6, i-16);
    }

    pieces[8]->modelPiece(0, 0);
    pieces[9]->modelPiece(0, 7);
    pieces[10]->modelPiece(0, 1);
    pieces[11]->modelPiece(0, 6);
    pieces[12]->modelPiece(0, 2);
    pieces[13]->modelPiece(0, 5);
    pieces[14]->modelPiece(0, 3);
    pieces[15]->modelPiece(0, 4);

    pieces[24]->modelPiece(7, 0);
    pieces[25]->modelPiece(7, 7);
    pieces[26]->modelPiece(7, 1);
    pieces[27]->modelPiece(7, 6);
    pieces[28]->modelPiece(7, 2);
    pieces[29]->modelPiece(7, 5);
    pieces[30]->modelPiece(7, 3);
    pieces[31]->modelPiece(7, 4);
}

void Objects::freeAll(){
    int k = 0;
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[k].freeBoard();
            k++;
        }
    }

    for (int i=0; i<32; i++){
        pieces[i]->freePiece();
    }
}

void Objects::occupyAll(){
    for (int i=0; i<32; i++){
        occupied[pieces[i]->number][0] = pieces[i]->piecePosition_x;
        occupied[pieces[i]->number][1] = pieces[i]->piecePosition_z;
    }
}

void Objects::uploadLight(Start* start) {
    const Light& light = *(start->light);
    glUniform4f(start->LaLocation, light.La.r, light.La.g, light.La.b, light.La.a);
    glUniform4f(start->LdLocation, light.Ld.r, light.Ld.g, light.Ld.b, light.Ld.a);
    glUniform4f(start->LsLocation, light.Ls.r, light.Ls.g, light.Ls.b, light.Ls.a);
    glUniform3f(start->lightPositionLocation, light.lightPosition_worldspace.x,
                light.lightPosition_worldspace.y, light.lightPosition_worldspace.z);
    glUniform1f(start->lightPowerLocation, light.power);
}
void Objects::uploadMaterial(const Material& mtl, Start* start) {
    glUniform4f(start->KaLocation, mtl.Ka.r, mtl.Ka.g, mtl.Ka.b, mtl.Ka.a);
    glUniform4f(start->KdLocation, mtl.Kd.r, mtl.Kd.g, mtl.Kd.b, mtl.Kd.a);
    glUniform4f(start->KsLocation, mtl.Ks.r, mtl.Ks.g, mtl.Ks.b, mtl.Ks.a);
    glUniform1f(start->NsLocation, mtl.Ns);
}

void Objects::depthPass(mat4 viewMatrix, mat4 projectionMatrix, Start* start) {

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, start->depthFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(start->depthProgram);
    mat4 view_projection = projectionMatrix * viewMatrix;
    glUniformMatrix4fv(start->shadowViewProjectionLocation, 1, GL_FALSE, &view_projection[0][0]);

    depthLoopAll();

    // for the platforms
//    glUniformMatrix4fv(start->shadowModelLocation, 1, GL_FALSE, &start->modelMatrixPlat1[0][0]);
//    start->plat1->bind();
//    start->plat1->draw();
//    glUniformMatrix4fv(start->shadowModelLocation, 1, GL_FALSE, &start->modelMatrixPlat2[0][0]);
//    start->plat2->bind();
//    start->plat2->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Objects::depthLoopAll(){
    for (int i=0; i<32; i++){
        if (pieces[i]->number != -1) {
            pieces[i]->depthLoopPiece();
        }
    }
//    int k = 0;
//    for(int i = 0; i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            mat4 modelMatrix = translate(mat4(), vec3(i, 0.0, j));
//            board[k].depthLoopBoard(modelMatrix);
//            k++;
//        }
//    }

}

void Objects::lightingPass(mat4 viewMatrix, mat4 projectionMatrix, Start* start) {

    // macOS
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, W_WIDTH, W_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(start->shaderProgram);

    glUniformMatrix4fv(start->viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(start->projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    uploadLight(start);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, start->depthTexture);
    glUniform1i(start->depthMapSampler, 0);

    mat4 light_VP = start->light->lightVP();
    glUniformMatrix4fv(start->lightVPLocation, 1, GL_FALSE, &light_VP[0][0]);

    lightLoopAll(start);
}

void Objects::lightLoopAll(Start* start){

    for (int i=0; i<16; i++){
        if (pieces[i]->number != -1) {
            uploadMaterial(pearl, start);
            glUniform1i(start->useTextureLocation, 0);

            pieces[i]->lightLoopPiece();
        }
    }

    for (int i=16; i<32; i++){
        if (pieces[i]->number != -1) {
            uploadMaterial(obsidian, start);
            glUniform1i(start->useTextureLocation, 0);

            pieces[i]->lightLoopPiece();
        }
    }

    // board
    int k = 0;
    int counter = 0;
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i + j) % 2 == 0){
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, start->blackTexture[start->list[counter]]);
                glUniform1i(start->diffuseColorSampler, 1);
                counter++;
            }
            else{
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, start->whiteTexture[start->list[counter]]);
                glUniform1i(start->diffuseColorSampler, 1);
                counter++;
            }
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, start->modelSpecularTexture);
            glUniform1i(start->specularColorSampler, 2);

            glUniform1i(start->useTextureLocation, 1);

            mat4 modelMatrix = translate(mat4(), vec3(i, 0.0, j));
            board[k].lightLoopBoard(modelMatrix);
            k++;
        }
    }

    // FOR THE TABLE
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, start->textureTable);
    glUniform1i(start->diffuseColorSampler, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, start->modelSpecularTexture);
    glUniform1i(start->specularColorSampler, 2);

    glUniform1i(start->useTextureLocation, 1);

    mat4 modelMatrix = translate(mat4(), vec3(3.5, 0.0, 3.5));
    glUniformMatrix4fv(start->modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    start->table->bind();
    start->table->draw();

    platforms(start->plat1, start->modelMatrixPlat1, start);
    platforms(start->plat2, start->modelMatrixPlat2, start);
    
}

void Objects::platforms(Drawable* plat, mat4 modelMatrix, Start* start){
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, start->texturePlat);
    glUniform1i(start->diffuseColorSampler, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, start->modelSpecularTexture);
    glUniform1i(start->specularColorSampler, 2);

    glUniform1i(start->useTextureLocation, 1);

 //    mat4 modelMatrixPlat1 = translate(mat4(), vec3(3.5, -0.15, -1.5));
    glUniformMatrix4fv(start->modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    plat->bind();
    plat->draw();
}


void Objects::createObjects(Start* start) {

    const char* path_object_pawn = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/pawn.obj";
    const char* path_object_tower = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/tower.obj";
    const char* path_object_horse ="/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/horse.obj";
    const char* path_object_bishop = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/bishop.obj";
    const char* path_object_queen = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/queen.obj";
    const char* path_object_king = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/king.obj";

    const char* path_texture_white = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/l.bmp";
    const char* path_texture_black = "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/e.bmp";


    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            board.push_back(Board(start, i, j));
        }
    }

    Pawn* pawnW1 = new Pawn(start, path_object_pawn, path_texture_white, 0, 0);
    Pawn* pawnW2 = new Pawn(start, path_object_pawn, path_texture_white, 0, 1);
    Pawn* pawnW3 = new Pawn(start, path_object_pawn, path_texture_white, 0, 2);
    Pawn* pawnW4 = new Pawn(start, path_object_pawn, path_texture_white, 0, 3);
    Pawn* pawnW5 = new Pawn(start, path_object_pawn, path_texture_white, 0, 4);
    Pawn* pawnW6 = new Pawn(start, path_object_pawn, path_texture_white, 0, 5);
    Pawn* pawnW7 = new Pawn(start, path_object_pawn, path_texture_white, 0, 6);
    Pawn* pawnW8 = new Pawn(start, path_object_pawn, path_texture_white, 0, 7);
    Tower* towerW1 = new Tower(start, path_object_tower, path_texture_white, 0, 8);
    Tower* towerW2 = new Tower(start, path_object_tower, path_texture_white, 0, 9);
    Horse* horseW1 = new Horse(start, path_object_horse, path_texture_white, 0, 10);
    Horse* horseW2 = new Horse(start, path_object_horse, path_texture_white, 0, 11);
    Bishop* bishopW1 = new Bishop(start, path_object_bishop, path_texture_white, 0, 12);
    Bishop* bishopW2 = new Bishop(start, path_object_bishop, path_texture_white, 0, 13);
    Queen* queenW = new Queen(start, path_object_queen, path_texture_white, 0, 14);
    King* kingW = new King(start, path_object_king, path_texture_white, 0, 15);

    Pawn* pawnB1 = new Pawn(start, path_object_pawn, path_texture_black, 1, 16);
    Pawn* pawnB2 = new Pawn(start, path_object_pawn, path_texture_black, 1, 17);
    Pawn* pawnB3 = new Pawn(start, path_object_pawn, path_texture_black, 1, 18);
    Pawn* pawnB4 = new Pawn(start, path_object_pawn, path_texture_black, 1, 19);
    Pawn* pawnB5 = new Pawn(start, path_object_pawn, path_texture_black, 1, 20);
    Pawn* pawnB6 = new Pawn(start, path_object_pawn, path_texture_black, 1, 21);
    Pawn* pawnB7 = new Pawn(start, path_object_pawn, path_texture_black, 1, 22);
    Pawn* pawnB8 = new Pawn(start, path_object_pawn, path_texture_black, 1, 23);
    Tower* towerB1 = new Tower(start, path_object_tower, path_texture_black, 1, 24);
    Tower* towerB2 = new Tower(start, path_object_tower, path_texture_black, 1, 25);
    Horse* horseB1 = new Horse(start, path_object_horse, path_texture_black, 1, 26);
    Horse* horseB2 = new Horse(start, path_object_horse, path_texture_black, 1, 27);
    Bishop* bishopB1 = new Bishop(start, path_object_bishop, path_texture_black, 1, 28);
    Bishop* bishopB2 = new Bishop(start, path_object_bishop, path_texture_black, 1, 29);
    Queen* queenB = new Queen(start, path_object_queen, path_texture_black, 1, 30);
    King* kingB = new King(start, path_object_king, path_texture_black, 1, 31);

    Piece* p[32] = {pawnW1, pawnW2, pawnW3, pawnW4, pawnW5, pawnW6, pawnW7, pawnW8,
                    towerW1, towerW2, horseW1, horseW2, bishopW1, bishopW2, queenW, kingW,
                    pawnB1, pawnB2, pawnB3, pawnB4, pawnB5, pawnB6, pawnB7, pawnB8,
                    towerB1, towerB2, horseB1, horseB2, bishopB1, bishopB2, queenB, kingB};

    for (int i = 0; i<32; i++){
        this->pieces[i] = p[i];
    }
}

int Objects::whichPiece(int x_piece, int z_piece){
    for (int i = 0; i<32; i++) {
        if ((x_piece == pieces[i]->piecePosition_x) && (z_piece == pieces[i]->piecePosition_z)) {
            return i;
        }
    }
    return -1;
}

void Objects::updatePosition(){
    pieces[number]->piecePosition_x = pieces[number]->x.x;
    pieces[number]->piecePosition_z = pieces[number]->x.z;

    pieces[eatenPiece]->piecePosition_x = pieces[eatenPiece]->x.x;
    pieces[eatenPiece]->piecePosition_z = pieces[eatenPiece]->x.z;
}

void Objects::startMove(int x_piece, int z_piece, int x, int z){

    if (number == 10 || number == 11 || number == 26 || number == 27){
        startMoveHorse(x_piece, z_piece, x, z);
    }
    else {
        x_start = x_piece;
        z_start = z_piece;
        x_end = x;
        z_end = z;

        if (x_piece == x) {
            vx0 = 0.0f;
        } else if (x_piece < x) {
            vx0 = 0.3f;
        } else if (x_piece > x) {
            vx0 = -0.3f;
        }

        if (z_piece == z) {
            vz0 = 0.0f;
        } else if (z_piece < z) {
            vz0 = 0.3f;
        } else if (z_piece > z) {
            vz0 = -0.3f;
        }

        vec3 v0 = vec3(vx0, 0, vz0);

        pieces[number]->x = vec3(pieces[number]->piecePosition_x, 0.1, pieces[number]->piecePosition_z);
        pieces[number]->v = v0;
        pieces[number]->P = pieces[number]->m * pieces[number]->v;
        pieces[number]->forcing = [&](float t, const vector<float> &y) -> vector<float> {
            vector<float> f(6, 0.0f);
            f[0] = 15.0f * vx0;
            f[1] = 0.0f;
            f[2] = 15.0f * vz0;
            return f;
        };

        middle = 0;
        arrived = 0;
        queen = 0;
    }
}



void Objects::startMoveHorse(int x_piece, int z_piece, int x, int z){

    x_start = x_piece;
    z_start = z_piece;
    x_end = x;
    z_end = z;

    if (x_piece + 2 == x){
        vx0 = 0.2f;
    }
    else if (x_piece + 1 == x) {
        vx0 = 0.1f;
    }
    else if (x_piece - 2 == x) {
        vx0 = -0.2f;
    }
    else if (x_piece - 1 == x) {
        vx0 = -0.1f;
    }

    if (z_piece + 2 == z){
        vz0 = 0.2f;
    }
    else if (z_piece + 1 == z) {
        vz0 = 0.1f;
    }
    else if (z_piece - 2 == z) {
        vz0 = -0.2f;
    }
    else if (z_piece - 1 == z) {
        vz0 = -0.1f;
    }

    vy0 = 4.0f;
    vec3 v0 = vec3(vx0, vy0, vz0);

    pieces[number]->x = vec3(pieces[number]->piecePosition_x, 0.1, pieces[number]->piecePosition_z);
    pieces[number]->v = v0;
    pieces[number]->P = pieces[number]->m * pieces[number]->v;
    pieces[number]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[0] = 5.0f * vx0;
        f[1] = -3.252f;
        f[2] = 5.0f * vz0;
        return f;
    };

    middle = 0;
    arrived = 0;
}

void Objects::middleMove(){

    if (number == 10 || number == 11 || number == 26 || number == 27){
        middleMoveHorse();
    }
    else {
        float dxTotal = abs(x_end - x_start);
        float dzTotal = abs(z_end - z_start);

        float dx = abs(pieces[number]->x.x - x_start);
        float dz = abs(pieces[number]->x.z - z_start);

        if (pieces[number]->v.x != 0 && (dx > dxTotal / 2.0f)) {
            endMove();
            middle = 1;
        }
        else if (pieces[number]->v.z != 0 && (dz > dzTotal / 2.0f)) {
            endMove();
            middle = 1;
        }
    }
}

void Objects::middleMoveHorse(){

    float dxTotal = abs(x_end - x_start);

    float dx = abs(pieces[number]->x.x - x_start);

    if (dx > dxTotal / 2.0f){
        endMoveHorse();
        middle = 1;
    }

}


void Objects::endMove(){

    pieces[number]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[0] = -15.0f * vx0;
        f[1] = 0.0f;
        f[2] = -15.0f * vz0;
        return f;
    };
}

void Objects::endMoveHorse(){

    pieces[number]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[0] = -5.0f * vx0;
        f[1] = -3.252f;
        f[2] = -5.0f * vz0;
        return f;
    };
}

void Objects::arrival(){

    if (number == 10 || number == 11 || number == 26 || number == 27){
        arrivalHorse();
    }
    else {
        float dxTotal = abs(x_end - x_start);
        float dzTotal = abs(z_end - z_start);

        float dx = abs(pieces[number]->x.x - x_start);
        float dz = abs(pieces[number]->x.z - z_start);

        if (pieces[number]->v.x != 0 && (dx > dxTotal)) {
            arrived = 1;
            finishMove();
        } else if (pieces[number]->v.z != 0 && (dz > dzTotal)) {
            arrived = 1;
            finishMove();
        }
    }

}

void Objects::arrivalHorse(){

    float dxTotal = abs(x_end - x_start);

    float dx = abs(pieces[number]->x.x - x_start);

    if (dx > dxTotal) {
        arrived = 1;
        finishMove();
    }
}

void Objects::finishMove(){
    pieces[number]->piecePosition_x = x_end;
    pieces[number]->piecePosition_z = z_end;
    pieces[number]->x.x = x_end;
    pieces[number]->x.z = z_end;
    pieces[number]->x.y = 0.1;
    pieces[number]->pieceModel = glm::translate(glm::mat4(), glm::vec3(pieces[number]->x.x, pieces[number]->x.y, pieces[number]->x.z)) * pieces[number]->pieceRotation;
    occupied[number][0] = x_end;
    occupied[number][1] = z_end;

    pieces[number]->v = vec3(0,0,0);
    pieces[number]->P = pieces[number]->m * pieces[number]->v;
    pieces[number]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[0] = 0.0f;
        f[1] = 0.0f;
        f[2] = 0.0f;
        return f;
    };
}


void Objects::startEat(int x_piece, int z_piece, int x, int z) {

    k = sqrt((x_piece - x) * (x_piece - x) + (z_piece - z) * (z_piece - z));

    if (number == 10 || number == 11 || number == 26 || number == 27){
        startEatHorse(x_piece, z_piece, x, z);
    }
    else {
        x_start = x_piece;
        z_start = z_piece;
        x_end = x;
        z_end = z;

        vy0 = 8.0f / k;

        if (x_piece == x) {
            vx0 = 0.0f;
        } else if (x_piece < x) {
            vx0 = k * 0.35f;
        } else if (x_piece > x) {
            vx0 = -k * 0.35f;
        }

        if (z_piece == z) {
            vz0 = 0.0f;
        } else if (z_piece < z) {
            vz0 = k * 0.35f;
        } else if (z_piece > z) {
            vz0 = -k * 0.35f;
        }

        vec3 v0 = vec3(vx0, vy0, vz0);

        pieces[number]->x = vec3(pieces[number]->piecePosition_x, 0.1, pieces[number]->piecePosition_z);
        pieces[number]->v = v0;
        pieces[number]->P = pieces[number]->m * pieces[number]->v;
        pieces[number]->forcing = [&](float t, const vector<float> &y) -> vector<float> {
            vector<float> f(6, 0.0f);
            f[0] = 0.0f;
            f[1] = -0.15f;
            f[2] = 0.0f;
            return f;
        };
    }

    int xFall;
    int zFall;
    if (vx0 != 0){
        xFall = x_end + ( vx0 / abs(vx0) );
    }
    else xFall = x_end;
    if (vz0 != 0){
        zFall = z_end + ( vz0 / abs(vz0) );
    }
    else zFall = z_end;

    c = 0;
    fallPiece = -1;
    for (int i=0; i<32; i++){
        if ( (pieces[i]->piecePosition_x == xFall) && (pieces[i]->piecePosition_z == zFall)){
            c = 1;
            fallPiece = i;
        }
    }

    if ((eatenPiece>7 && eatenPiece<16) || (eatenPiece>23 && eatenPiece<32)) {
        int xFall;
        int zFall;
        if (vx0 != 0) {
            xFall = x_end + 2 * (vx0 / abs(vx0));
        } else xFall = x_end;
        if (vz0 != 0) {
            zFall = z_end + 2 * (vz0 / abs(vz0));
        } else zFall = z_end;

        cTwo = 0;
        fallPieceTwo = -1;
        for (int i = 0; i < 32; i++) {
            if ((pieces[i]->piecePosition_x == xFall) && (pieces[i]->piecePosition_z == zFall)) {
                cTwo = 1;
                fallPieceTwo = i;
            }
        }
    }

    if (c == 1) {
        pieces[fallPiece]->v = vec3(0, 10.0f/k, 0);
        pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
    }
    else if (c == 0) {}

    if (cTwo == 1) {
        pieces[fallPieceTwo]->v = vec3(0, 10.0f/k, 0);
        pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
    }
    else if (cTwo == 0) {}

    middle = 0;
    arrived = 0;
    fell = 0;
    breaking = 0;
    created = 0;
    down = 1;
    endBreak = 0;
    queen = 0;
}

void Objects::startEatHorse(int x_piece, int z_piece, int x, int z) {
    x_start = x_piece;
    z_start = z_piece;
    x_end = x;
    z_end = z;

    if (x_piece + 2 == x){
        vx0 = 0.8f;
    }
    else if (x_piece + 1 == x) {
        vx0 = 0.4f;
    }
    else if (x_piece - 2 == x) {
        vx0 = -0.8f;
    }
    else if (x_piece - 1 == x) {
        vx0 = -0.4f;
    }

    if (z_piece + 2 == z){
        vz0 = 0.8f;
    }
    else if (z_piece + 1 == z) {
        vz0 = 0.4f;
    }
    else if (z_piece - 2 == z) {
        vz0 = -0.8f;
    }
    else if (z_piece - 1 == z) {
        vz0 = -0.4f;
    }

    vy0 = 2.0f;
    vec3 v0 = vec3(vx0, vy0, vz0);

    pieces[number]->x = vec3(pieces[number]->piecePosition_x, 0.1, pieces[number]->piecePosition_z);
    pieces[number]->v = v0;
    pieces[number]->P = pieces[number]->m * pieces[number]->v;

}


void Objects::middleEat(){

    if ((pieces[number]->x.y - 0.1f) >= (pieces[eatenPiece]->yUp - pieces[number]->yDown - 0.025)){

        pieces[number]->v.y = 0.0f;
        pieces[number]->P.y = pieces[number]->m * pieces[number]->v.y;
        pieces[number]->forcing = [&](float t, const vector<float> &y) -> vector<float> {
            vector<float> f(6, 0.0f);
            f[0] = 0.0f;
            f[1] = 0.0f;
            f[2] = 0.0f;
            return f;
        };
        middle = 1;
    }

    if (c == 1) {
        if (pieces[fallPiece]->x.y >= 2.2) {
            pieces[fallPiece]->v = vec3(0, 0, 0);
            pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
        }
    }
    if (cTwo == 1) {
        if (pieces[fallPieceTwo]->x.y >= 2.2) {
            pieces[fallPieceTwo]->v = vec3(0, 0, 0);
            pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
        }
    }
}

void Objects::eatArrival(){

    float d = sqrt((pieces[number]->x.x - pieces[eatenPiece]->x.x) * (pieces[number]->x.x - pieces[eatenPiece]->x.x) +
                          (pieces[number]->x.z - pieces[eatenPiece]->x.z) * (pieces[number]->x.z - pieces[eatenPiece]->x.z));


    if (eatenPiece == 10 || eatenPiece == 11 || eatenPiece == 26 || eatenPiece == 27){

        if (vz0 == 0){
            if (d <= (pieces[number]->rDown + pieces[eatenPiece]->xUp)) {
                arrived = 1;
                startFall();
            }
        }
        else if (vx0 == 0){
            if (d <= (pieces[number]->rDown + pieces[eatenPiece]->zUp)) {
                arrived = 1;
                startFall();
            }
        }
        else {
            if (d <= (pieces[number]->rDown + pieces[eatenPiece]->zUp)) {
                arrived = 1;
                startFall();
            }
        }
    }
    else {
        if (d <= (pieces[number]->rDown + pieces[eatenPiece]->rUp)) {
            arrived = 1;
            startFall();
        }
    }

    if (c == 1) {
        if (pieces[fallPiece]->x.y >= 2.2) {
            pieces[fallPiece]->v = vec3(0, 0, 0);
            pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
        }
    }
    if (cTwo == 1) {
        if (pieces[fallPieceTwo]->x.y >= 2.2) {
            pieces[fallPieceTwo]->v = vec3(0, 0, 0);
            pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
        }
    }
}

void Objects::startFall(){

    wx = 0.0f;
    wz = 0.0f;

    if (number == 10 || number == 11 || number == 26 || number == 27){
        startFallHorse();
    }
    else {
        float nvx = 0.0f;
        float nvz = 0.0f;

        if (vx0 != 0) {
            nvx = (vx0 / abs(vx0)) * 0.6f;
            wz = (vx0 / abs(vx0));
        }
        if (vz0 != 0) {
            nvz = (vz0 / abs(vz0)) * 0.6f;
            wx = (vz0 / abs(vz0));
        }

        pieces[number]->v = vec3(nvx, 0, nvz);
        pieces[number]->P = pieces[number]->m * pieces[number]->v;
    }

    if (c == 1) {
        pieces[fallPiece]->v = vec3(0, 0, 0);
        pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
    }
    if (cTwo == 1) {
        pieces[fallPieceTwo]->v = vec3(0, 0, 0);
        pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
    }

    pieces[eatenPiece]->w = vec3(wx*0.4 , 0, -wz*0.4 );
    pieces[eatenPiece]->L = inverse(pieces[eatenPiece]->I_inv) * pieces[eatenPiece]->w;

    pieces[eatenPiece]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[3] = wx;
        f[5] = -wz;
        return f;
    };

    time = glfwGetTime();

}

void Objects::startFallHorse(){
    float nvx = 0.0f;
    float nvz = 0.0f;

    if (abs(vx0) > abs(vz0)) {
        nvx = (vx0 / abs(vx0)) * 0.6f;
        nvz = (vz0 / abs(vz0)) * 0.3f;
        wz = (vx0 / abs(vx0));
        wx = (vz0 / abs(vz0));
    }
    else if (abs(vz0) > abs(vx0)) {
        nvz = (vz0 / abs(vz0)) * 0.6f;
        nvx = (vx0 / abs(vx0)) * 0.3f;
        wx = (vz0 / abs(vz0));
        wz = (vx0 / abs(vx0));
    }

    pieces[number]->v = vec3(nvx, 0, nvz);
    pieces[number]->P = pieces[number]->m * pieces[number]->v;
}


void Objects::endFall(){

    if ( (abs(pieces[number]->x.x - x_end) < 0.05) && (abs(pieces[number]->x.z - z_end) < 0.05) ) {
        pieces[number]->x.x = x_end;
        pieces[number]->x.z = z_end;
        pieces[number]->v = vec3(0,0,0);
        pieces[number]->P = pieces[number]->m * pieces[number]->v;
    }

    float currentTime = glfwGetTime();

    if (wx!=0 && wz!=0) {
        if ((1.25 <= (currentTime - time)) && (arrived == 1)) {
            fell = 1;
            finishEat();
        }
    }
    else{
        if ((1.5 <= (currentTime - time)) && (arrived == 1)) {
            fell = 1;
            finishEat();
        }
    }

}


void Objects::finishEat(){

    model = pieces[eatenPiece]->pieceModel;

    pieces[eatenPiece]->w = vec3(0 ,0, 0);
    pieces[eatenPiece]->L = inverse(pieces[eatenPiece]->I_inv) * pieces[eatenPiece]->w;

    pieces[eatenPiece]->forcing = [&](float t, const vector<float>& y)->vector<float> {
        vector<float> f(6, 0.0f);
        f[3] = 0;
        f[5] = 0;
        return f;
    };

    breaking = 1;
}

void Objects::breakPiece(Start* start){

    pieces[eatenPiece]->number = -1;
    
    const char* path_object = pieces[eatenPiece]->path_object;
    if (created == 0){
        brokenPiece = new Piece(start, path_object, path_object, pieces[eatenPiece]->color, eatenPiece);
        brokenPiece->createPiece();
        created = 1;
        timeStartBreak = glfwGetTime();
    }
    timeBreak = (glfwGetTime() - timeStartBreak);

    glUseProgram(start->breakProgram);

    MVPMatrix = start->camera->projectionMatrix * start->camera->viewMatrix * model;

    glUniformMatrix4fv(start->MVPLoc, 1, GL_FALSE, &MVPMatrix[0][0]);

    if (pieces[eatenPiece]->color == 0){
        glUniform4f(start->colorLoc, colorPearl.r, colorPearl.g, colorPearl.b, colorPearl.a);
    }
    else if(pieces[eatenPiece]->color == 1){
        glUniform4f(start->colorLoc, colorObsidian.r, colorObsidian.g, colorObsidian.b, colorObsidian.a);
    }

    glUniform1f(start->timeLoc, timeBreak);

    glBindVertexArray(brokenPiece->pieceVAO);
    glDrawArrays(GL_TRIANGLES, 0, brokenPiece->pieceVertices.size());

    if (timeBreak > 2.5){
        breaking = 0;
        down = 0;
        c1 = 0;
        c2 = 0;
        c3 = 0;
        startDown();
    }

}

void Objects::startDown(){

    pieces[number]->v.y = -2.0f;
    pieces[number]->P = pieces[number]->m * pieces[number]->v;

    if (c == 1) {
        pieces[fallPiece]->v.y = -2.0f;
        pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
    }
    if (cTwo == 1) {
        pieces[fallPieceTwo]->v.y = -2.0f;
        pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
    }

}

void Objects::finishBreak(){

    if (pieces[number]->x.y <= 0.1){
        pieces[number]->v.y = 0.0f;
        pieces[number]->P = pieces[number]->m * pieces[number]->v;
        pieces[number]->x.y = 0.1;
        pieces[number]->pieceModel = translate(mat4(), vec3(pieces[number]->x.x,
                                                            pieces[number]->x.y,
                                                            pieces[number]->x.z)) *
                                                            pieces[number]->pieceRotation;
        c1 = 1;
    }
    if (c == 1) {
        if (pieces[fallPiece]->x.y <= 0.1) {
            pieces[fallPiece]->x.y = 0.1;
            pieces[fallPiece]->v.y = 0.0f;
            pieces[fallPiece]->P = pieces[fallPiece]->m * pieces[fallPiece]->v;
            pieces[fallPiece]->pieceModel = translate(mat4(), vec3(pieces[fallPiece]->x.x,
                                                                   pieces[fallPiece]->x.y,
                                                                   pieces[fallPiece]->x.z)) *
                                                                   pieces[fallPiece]->pieceRotation;
            c2 = 1;
        }
    }
    if (cTwo == 1) {
        if (pieces[fallPieceTwo]->x.y <= 0.1) {
            pieces[fallPieceTwo]->x.y = 0.1;
            pieces[fallPieceTwo]->v.y = 0.0f;
            pieces[fallPieceTwo]->P = pieces[fallPieceTwo]->m * pieces[fallPieceTwo]->v;
            pieces[fallPieceTwo]->pieceModel = translate(mat4(), vec3(pieces[fallPieceTwo]->x.x,
                                                                   pieces[fallPieceTwo]->x.y,
                                                                   pieces[fallPieceTwo]->x.z)) *
                                                                   pieces[fallPieceTwo]->pieceRotation;
            c3 = 1;
        }
    }

    if ((c1==1) && (c2==c) && (c3==cTwo)) {
        endBreak = 1;
        pieces[eatenPiece]->number = eatenPiece;

        if (pieces[eatenPiece]->color == 1){
            if (eatenWhite < 8){
                pieces[eatenPiece]->x.z = -1;
                pieces[eatenPiece]->x.x = eatenWhite;
                eatenWhite++;
            }
            else{
                pieces[eatenPiece]->x.z = -2;
                pieces[eatenPiece]->x.x = eatenWhite - 8;
                eatenWhite++;
            }
        }
        if (pieces[eatenPiece]->color == 0){
            if (eatenBlack > -1){
                pieces[eatenPiece]->x.z = 8;
                pieces[eatenPiece]->x.x = eatenBlack;
                eatenBlack--;
            }
            else{
                pieces[eatenPiece]->x.z = 9;
                pieces[eatenPiece]->x.x = eatenBlack + 8;
                eatenBlack--;
            }
        }
        updatePosition();
        pieces[eatenPiece]->pieceModel = translate(mat4(), vec3(pieces[eatenPiece]->x.x,
                                                                pieces[eatenPiece]->x.y,
                                                                pieces[eatenPiece]->x.z)) *
                                                                pieces[eatenPiece]->pieceRotation;
        occupied[number][0] = x_end;
        occupied[number][1] = z_end;
        occupied[eatenPiece][0] = pieces[eatenPiece]->piecePosition_x;
        occupied[eatenPiece][1] = pieces[eatenPiece]->piecePosition_z;

        down = 1;
        cTwo = 0;
        fallPieceTwo = -1;

        brokenPiece->freePiece();

    }
}


void Objects::pawnToQueen(Start* start, float dt){

    int queenDead = 0;
    if (((pieces[14]->x.z > 7) || (pieces[14]->x.z < 0)) && (pieces[number]->color == 0)) queenDead = 1;
    if (((pieces[30]->x.z > 7) || (pieces[30]->x.z < 0)) && (pieces[number]->color == 1)) queenDead = 1;

    if ( ((((number > -1) && (number < 8) && (pieces[number]->x.x == 7)) ||
         ((number > 15) && (number < 24) && (pieces[number]->x.x == 0))) && queenDead) || (newParticleSystem == 0)) {
        if (newParticleSystem == 1) {
            particleSystem = new ParticleSystem(start);
            particleSystem->createParticles();
            newParticleSystem = 0;
            queen = 0;
            timeStartSmoke = glfwGetTime();
            transfer = translate(mat4(), vec3(pieces[number]->x.x,0,pieces[number]->x.z));
        }
        else if (newParticleSystem == 0) {
            float currentTime = glfwGetTime();

            if (currentTime - timeStartSmoke < 5) {
                for (int i = 0; i < 70; i++) {
                    particleSystem->createParticles();
                }
            }

            particleSystem->drawParticles(transfer);
            particleSystem->update(dt);

            if ((currentTime - timeStartSmoke > 5) && (queenDead == 1)){

                int k;
                if (pieces[number]->color == 0) {
                    k = 14;
                }
                else if (pieces[number]->color == 1) {
                    k = 30;
                }

                int queenX = pieces[k]->x.x;
                int queenZ = pieces[k]->x.z;
                mat4 queenModel = pieces[k]->pieceModel;

                pieces[k]->x.x = pieces[number]->x.x;
                pieces[k]->x.z = pieces[number]->x.z;
                pieces[k]->piecePosition_x = pieces[number]->x.x;
                pieces[k]->piecePosition_z = pieces[number]->x.z;
                pieces[k]->pieceModel = pieces[number]->pieceModel;
                occupied[k][0] = pieces[number]->x.x;
                occupied[k][1] = pieces[number]->x.z;

                pieces[number]->x.x = queenX;
                pieces[number]->x.z = queenZ;
                pieces[number]->piecePosition_x = queenX;
                pieces[number]->piecePosition_z = queenZ;
                pieces[number]->pieceModel = queenModel;
                occupied[number][0] = queenX;
                occupied[number][1] = queenZ;

            }

            if (currentTime - timeStartSmoke > 9.5) {
                queen = 1;
                newParticleSystem = 1;
            }
        }
    }
    else{
        queen = 1;
    }
}




