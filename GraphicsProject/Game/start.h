#ifndef CHESS_START_H
#define CHESS_START_H

#include <GL/glew.h>
#include <glfw3.h>
#include <common/camera.h>
#include <common/light.h>
#include <common/model.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Start{
public:
    GLFWwindow* window;
    Camera* camera;
    Light* light;

    GLuint shaderProgram;

    GLuint depthFrameBuffer;
    GLuint depthTexture;
    GLuint depthProgram;

    GLuint particleProgram;
    GLuint MVPLocation;
    GLuint colorLocation;

    GLuint breakProgram;
    GLuint MVPLoc;
    GLuint colorLoc;
    GLuint timeLoc;

    GLuint viewMatrixLocation;
    GLuint projectionMatrixLocation;
    GLuint modelMatrixLocation;
    GLuint KaLocation, KdLocation, KsLocation, NsLocation;
    GLuint LaLocation, LdLocation, LsLocation;
    GLuint lightPositionLocation;
    GLuint lightPowerLocation;
    GLuint diffuseColorSampler;
    GLuint specularColorSampler;
    GLuint useTextureLocation;
    GLuint depthMapSampler;
    GLuint lightVPLocation;

    GLuint shadowViewProjectionLocation;
    GLuint shadowModelLocation;

    GLuint textureSquare, textureTable, modelSpecularTexture;
    GLuint texturePlat;
    std::vector<GLuint> whiteTexture;
    std::vector<GLuint> blackTexture;
    std::vector<int> list;

    Drawable* table;
    Drawable* plat1;
    Drawable* plat2;
    mat4 modelMatrixPlat1;
    mat4 modelMatrixPlat2;
//    Drawable* plat3;
//    Drawable* plat4;

    void initialize();
    void createContext();
};

#endif //CHESS_START_H
