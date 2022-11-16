#include "start.h"
#include <GL/glew.h>
#include <glfw3.h>
#include <string>
#include <common/util.h>
#include <common/shader.h>
#include <common/texture.h>

#define W_WIDTH 1024
#define W_HEIGHT 768
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
#define TITLE "Chess"


void Start::initialize() {
    // Initialize GLFW
    if (!glfwInit()) {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
                                   " If you have an Intel GPU, they are not 3.3 compatible." +
                                   "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // Log
    logGLParameters();

    // Create camera
    camera = new Camera(window);
    camera->position = vec3(-3, 10, 3.5);
    camera->up = vec3(1, 0, 0);

    // Create light
    light = new Light(window,
                      vec4{ 1.0, 1.0, 1.0, 1.0 },
                      vec4{ 1, 1, 1, 1 },
                      vec4{ 1, 1, 1, 1 },
                      vec3{ 3.5, 8, 3.5 },
                      50.0f
    );

}

void Start::createContext() {

    shaderProgram = loadShaders(
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/vertex.vertexshader",
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/fragment.fragmentshader");
    depthProgram = loadShaders(
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Depth.vertexshader",
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Depth.fragmentshader");

    particleProgram = loadShaders(
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/particle.vertexshader",
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/particle.fragmentshader");

    breakProgram = loadShaders(
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/break.vertexshader",
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/break.fragmentshader",
            "/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/geometry.shader");


    projectionMatrixLocation = glGetUniformLocation(shaderProgram, "P");
    viewMatrixLocation = glGetUniformLocation(shaderProgram, "V");
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "M");

    KaLocation = glGetUniformLocation(shaderProgram, "mtl.Ka");
    KdLocation = glGetUniformLocation(shaderProgram, "mtl.Kd");
    KsLocation = glGetUniformLocation(shaderProgram, "mtl.Ks");
    NsLocation = glGetUniformLocation(shaderProgram, "mtl.Ns");

    LaLocation = glGetUniformLocation(shaderProgram, "light.La");
    LdLocation = glGetUniformLocation(shaderProgram, "light.Ld");
    LsLocation = glGetUniformLocation(shaderProgram, "light.Ls");
    lightPositionLocation = glGetUniformLocation(shaderProgram, "light.lightPosition_worldspace");
    lightPowerLocation = glGetUniformLocation(shaderProgram, "light.power");

    diffuseColorSampler = glGetUniformLocation(shaderProgram, "diffuseColorSampler");
    specularColorSampler = glGetUniformLocation(shaderProgram, "specularColorSampler");

    useTextureLocation = glGetUniformLocation(shaderProgram, "useTexture");

    depthMapSampler = glGetUniformLocation(shaderProgram, "shadowMapSampler");
    lightVPLocation = glGetUniformLocation(shaderProgram, "lightVP");

    shadowViewProjectionLocation = glGetUniformLocation(depthProgram, "VP");
    shadowModelLocation = glGetUniformLocation(depthProgram, "M");

    srand (2*time(NULL));
    for (int i = 0; i<64; i++) {
        list.push_back(rand() % 8);
    }

    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white1.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white2.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white3.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white4.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white5.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white6.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white7.bmp"));
    whiteTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/white8.bmp"));

    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black1.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black2.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black3.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black4.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black5.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black6.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black7.bmp"));
    blackTexture.push_back(loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black8.bmp"));

    textureSquare = loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/sq.bmp");
    modelSpecularTexture = loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black.bmp");

    // particle program
    MVPLocation = glGetUniformLocation(particleProgram, "MVP");
    colorLocation = glGetUniformLocation(particleProgram, "vertexColor");

    // break program
    MVPLoc = glGetUniformLocation(breakProgram, "MVP");
    colorLoc = glGetUniformLocation(breakProgram, "vertexColor");
    timeLoc = glGetUniformLocation(breakProgram, "time");

    // TABLE
    table = new Drawable("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/table.obj");
    textureTable = loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/table.bmp");
    modelSpecularTexture = loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/black.bmp");

    // PLATFORMS
    plat1 = new Drawable("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/platform.obj");
    plat2 = new Drawable("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/platform.obj");
//    plat3 = new Drawable("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/platform2.obj");
//    plat4 = new Drawable("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Models/platform2.obj");
    texturePlat = loadSOIL("/Users/antoniskefallonitis/Documents/Programming/c++/GraphicspProject/GraphicsProject/Textures/marble.bmp");

    modelMatrixPlat1 = translate(mat4(), vec3(3.5, -0.15, -1.5));
    modelMatrixPlat2 = translate(mat4(), vec3(3.5, -0.15, 8.5));

    // Create a depth framebuffer and a texture to store the depthmap - //

    // generate a framebuffer
    glGenFramebuffers(1, &depthFrameBuffer);
    // Binding the framebuffer, all changes bellow will affect the binded framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

    // We need a texture to store the depth image
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Set color to set out of border
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attaching the texture to the framebuffer, so that it will monitor the depth component
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glfwTerminate();
        throw runtime_error("Frame buffer not initialized correctly");
    }

    // Binding the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


