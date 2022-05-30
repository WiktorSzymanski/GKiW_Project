#include <iostream>
#include <sstream>
#include <stdlib.h>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Camera.h"
#include "Mesh.h"


const char* APP_TITLE = "OpenGL";
int gWindowWidth = 1024;
int gWindowHeight = 768;
bool gFullScreen = false;
bool glWireFrame = false;
GLFWwindow* gWindow = NULL;

OrbitCamera orbitCamera;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.f;
const float MOUSE_SENSIVITY = 0.25f;

bool gRenderAll = true; // --------------------------------------------------------------------------------------------------------------------------------
bool gRenderPlanets = true;

glm::vec3 gLook;
int gCurrentViewIndex = 8;
glm::vec3 gLookMargin[] = {
    glm::vec3(0.f, 22.4f ,0.f),
    glm::vec3(0.f, 20.6f ,0.f),
    glm::vec3(0.f, 20.0f ,0.f),
    glm::vec3(0.f, 19.4f ,0.f),
    glm::vec3(0.f, 19.4f ,0.f),
    glm::vec3(0.f, 19.4f ,0.f),
    glm::vec3(0.f, 19.4f ,0.f),
    glm::vec3(0.f, 18.6f ,0.f),
    glm::vec3(0.f, 17.4f ,0.f),
    glm::vec3(0.f, 15.0f ,0.f),
    glm::vec3(0.f, 13.4f ,0.f),
    glm::vec3(0.f,  9.6f ,0.f),
    glm::vec3(0.f,  8.4f ,0.f)
};


int gRocketStage = 0;

glm::vec3 gRocketPartsCurrentPos[] = { 
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f),
    glm::vec3(0.0f, 3.0f, 0.0f)
};
glm::vec3 gRocketPartsCurrentVelocity[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};
glm::vec3 gRocketPartsCurrentForce[] = { 
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

glm::vec3 gRocketPartsCurrentRoatationDir[] = {
    glm::vec3( rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(1.0f, 0.0f, -1.0f),
    glm::vec3(-1.0f, 0.0f, 1.0f),
    glm::vec3(1.0f, 0.0f, 1.0f),
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
    glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1)
};

float gRocketPartsCurrentRotationAngle[] = {
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f
};
float gRocketPartsCurrentRotationHolder[] = {
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f
};

//FPSCamera fpsCamera(glm::vec3(.0f, 2.0f, 5.0f));
//const double ZOOM_SENSIVITY = -3.0;
//const float MOVE_SPEED = 5.0f; //units per second
//const float MOUSE_SENSIVITY = 0.1f;

void setLook() {
    gLook = gRocketPartsCurrentPos[gCurrentViewIndex] + gLookMargin[gCurrentViewIndex];
}

void detatch() {
    switch (gRocketStage) {
    case 1:
        gRocketPartsCurrentForce[12] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f,  -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        gRocketPartsCurrentForce[11] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f,  -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);

        gRocketPartsCurrentRotationAngle[12] = (float(rand())  / float((RAND_MAX))) * 6 - 3.0f;
        gRocketPartsCurrentRotationAngle[11] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        break;
    case 2:
        gRocketPartsCurrentForce[10] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        gRocketPartsCurrentForce[9] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        gRocketPartsCurrentForce[0] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);

        gRocketPartsCurrentRotationAngle[10] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        gRocketPartsCurrentRotationAngle[9] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        gRocketPartsCurrentRotationAngle[0] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f; //------------------------------------------------------
        break;
    case 3:
        gRocketPartsCurrentForce[8] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        gRocketPartsCurrentForce[7] = glm::vec3(.5f, -3.0f, -.5f);
        gRocketPartsCurrentForce[6] = glm::vec3(-.5f, -3.0f, .5f);
        gRocketPartsCurrentForce[5] = glm::vec3(.5f, -3.0f, .5f);
        gRocketPartsCurrentForce[4] = glm::vec3(-.5f, -3.0f, -.5f);

        gRocketPartsCurrentRotationAngle[8] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        gRocketPartsCurrentRotationAngle[7] = 5.0f;
        gRocketPartsCurrentRotationAngle[6] = 5.0f;
        gRocketPartsCurrentRotationAngle[5] = -5.0f;
        gRocketPartsCurrentRotationAngle[4] = -5.0f;
        break;
    }
}

void applyForces(glm::vec3 F, double deltaTime, int partIndex) {
    float m = 1;
    glm::vec3 a = F / m;
    
    gRocketPartsCurrentVelocity[partIndex] += a * (float)deltaTime;

    gRocketPartsCurrentPos[partIndex] += gRocketPartsCurrentVelocity[partIndex] * (float)deltaTime;
}

void glfw_OnMouseScroll(GLFWwindow* window, double deltaX, double deltaY) {
    /*
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.setFOV((float)fov);
    */
};

void update(double elapsedTime) {
    /*
    double mouseX, mouseY;

    glfwGetCursorPos(gWindow, &mouseX, &mouseY);

    fpsCamera.rotate((float)(gWindowWidth / 2.0 - mouseX) * MOUSE_SENSIVITY, (float)(gWindowHeight / 2.0 - mouseY) * MOUSE_SENSIVITY);

    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());

    if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
    else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());

    if (glfwGetKey(gWindow, GLFW_KEY_Z) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
    else if (glfwGetKey(gWindow, GLFW_KEY_X) == GLFW_PRESS)
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
        */
};

void glfw_OnMouseMove(GLFWwindow* window, double posX, double posY) {
    static glm::vec2 lastMousePos = glm::vec2(0, 0);

    if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1) {
        gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSIVITY;
        gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSIVITY;
    }

    if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
        float dx = 0.01f * ((float)posX - lastMousePos.x);
        float dy = 0.01f * ((float)posY - lastMousePos.y);
        gRadius += dx - dy;
    }

    lastMousePos.x = (float)posX;
    lastMousePos.y = (float)posY;
    
};

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
    gWindowWidth = width;
    gWindowHeight = height;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
};

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        glWireFrame = !glWireFrame;

        if (glWireFrame) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        for (int i = 0; i < 13; i++) {
            gRocketPartsCurrentForce[i].y = 3.0f;
        }
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS && gRocketStage < 3) {
        gRocketStage += 1;
        detatch();
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if (gCurrentViewIndex == 12)
            gCurrentViewIndex = 0;
        else
            gCurrentViewIndex++;
    }

    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if (gCurrentViewIndex == 0)
            gCurrentViewIndex = 12;
        else
            gCurrentViewIndex--;
    }
}

void showFPS(GLFWwindow* window) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime();

    elapsedSeconds = currentSeconds - previousSeconds;

    // limit text update 4 times per second

    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed
            << APP_TITLE << "   "
            << "FPS: " << fps << "   "
            << "Frame Time: " << msPerFrame << " (ms)";

        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}

bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    if (gFullScreen) {
        GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);

        if (pVmode != NULL) {
            gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
        }

    }
    else {
        gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

    }

    if (gWindow == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);
    
    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetCursorPosCallback(gWindow, glfw_OnMouseMove);
    glfwSetScrollCallback(gWindow, glfw_OnMouseScroll);

    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //hides cursor
    glfwSetCursorPos(gWindow, gWindowWidth / 2.0, gWindowHeight / 2.0);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed" << std::endl;
        return false;
    }

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
    glViewport(0, 0, gWindowWidth, gWindowHeight);
    glEnable(GL_DEPTH_TEST);

    return true;
}

int main()
{
    if (!initOpenGL()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    srand((unsigned int)time(NULL));

    Mesh skyBox;
    skyBox.loadOBJ("../models/cube.obj");

    Texture3D skyBoxTex;

    
    std::vector<std::string> facesSkyBox = {
        "../textures/skyBox/right.png",
        "../textures/skyBox/left.png",
        "../textures/skyBox/top.png",
        "../textures/skyBox/bot.png",
        "../textures/skyBox/front.png",
        "../textures/skyBox/back.png"
    };
    
    /*
    std::vector<std::string> facesSkyBox = {
        "../textures/finalSpace/right.png",
        "../textures/finalSpace/left.png",
        "../textures/finalSpace/up.png",
        "../textures/finalSpace/down.png",
        "../textures/finalSpace/middle.png",
        "../textures/finalSpace/back.png"
    };
    */

    skyBoxTex.loadTexture(facesSkyBox);

    // Rocket
    Mesh saturnV[13];
    Texture2D saturnTex;

    glm::vec3 satScale(0.2f, 0.2f, 0.2f);

    float satSpeed[13];

    Mesh floor;
    Texture2D floorTex;

    glm::vec3 floorPos(0.0f, 0.0f, 0.0f);
    glm::vec3 floorScale(10.0f, 0.0f, 10.0f);

    Mesh launcher;
    Texture2D launcherTex;
    
    glm::vec3 launcherPos(0.0f, 0.0f, 0.0f);
    glm::vec3 launcherScale(.1f, .1f, .1f);

    Mesh lightMesh;
    lightMesh.loadOBJ("../models/light.obj");

    Mesh moon;
    Texture2D moonTex;

    Mesh earth;
    Texture2D earthTex;

    if (gRenderPlanets) {
        moon.loadOBJ("../models/moon.obj");
        moonTex.loadTexture("../textures/moon/Material _50_albedo.jpg");

        earth.loadOBJ("../models/earth.obj");
        earthTex.loadTexture("../textures/earth/land.png");
    }

    glm::vec3 moonPos(-210.f, 8000.f, -250.f);
    glm::vec3 moonScale(200.f, 200.f, 200.f);

    glm::vec3 earthPos(0.f, -3045.f, 0.f);
    glm::vec3 earthScale(3000.f, 3000.f, 3000.f);

    if (gRenderAll) {
        

        saturnV[0].loadOBJ("../models/betSat/Saturn V_0.obj");
        saturnV[1].loadOBJ("../models/betSat/Saturn V_1.obj");
        saturnV[2].loadOBJ("../models/betSat/Saturn V_2.obj");
        saturnV[3].loadOBJ("../models/betSat/Saturn V_3.obj");
        saturnV[4].loadOBJ("../models/betSat/Saturn V_4_1.obj");
        saturnV[5].loadOBJ("../models/betSat/Saturn V_4_2.obj");
        saturnV[6].loadOBJ("../models/betSat/Saturn V_4_3.obj");
        saturnV[7].loadOBJ("../models/betSat/Saturn V_4_4.obj");
        saturnV[8].loadOBJ("../models/betSat/Saturn V_5.obj");
        saturnV[9].loadOBJ("../models/betSat/Saturn V_6.obj");
        saturnV[10].loadOBJ("../models/betSat/Saturn V_7.obj");
        saturnV[11].loadOBJ("../models/betSat/Saturn V_8.obj");
        saturnV[12].loadOBJ("../models/betSat/Saturn V_9.obj");

        
        saturnTex.loadTexture("../textures/Saturn_V_Texture.png", true);


        launcher.loadOBJ("../models/ML_HP.obj");
        launcherTex.loadTexture("../textures/ML_HP_Tex.jpg", true);

        floor.loadOBJ("../models/floor.obj");
        floorTex.loadTexture("../textures/tile_floor.jpg", true);
    };

    
    
    Mesh testCube;
    testCube.loadOBJ("../models/cube.obj");

    ShaderProgram lightShader;
    lightShader.loadShaders("basic.vert", "basic.frag");

    //ShaderProgram lightingShader;
    //lightingShader.loadShaders("lighting.vert", "lighting.frag");

    ShaderProgram lightingShader;
    lightingShader.loadShaders("lighting_dir.vert", "lighting_dir.frag");

    ShaderProgram skyBoxShader;
    skyBoxShader.loadShaders("skyBox.vert", "skyBox.frag");

    skyBoxShader.use();
    skyBoxShader.setUniformSampler("skyBox", 0);

    double lastTime = glfwGetTime();
    //float angle = 0.0f;
    int multi = 1;


    // Main loop
    while (!glfwWindowShouldClose(gWindow)) {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        glfwPollEvents();
        update(deltaTime);
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model(1.0), view(1.0), projection(1.0);

        setLook();
        orbitCamera.setLookAt(gLook);
        orbitCamera.rotate(gYaw, gPitch);
        orbitCamera.setRadius(gRadius);

        view = orbitCamera.getViewMatrix();

        //view = fpsCamera.getViewMatrix();

        projection = glm::perspective(glm::radians(45.0f), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 20000.0f);
        //projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);


        //glm::vec3 viewPos;
        //viewPos.x = fpsCamera.getPosition().x;
        //viewPos.y = fpsCamera.getPosition().y;
        //viewPos.z = fpsCamera.getPosition().z;


        glm::vec3 viewPos;
        viewPos.x = orbitCamera.getPosition().x;
        viewPos.y = orbitCamera.getPosition().y;
        viewPos.z = orbitCamera.getPosition().z;

        // the light
        glm::vec3 lightPos = gRocketPartsCurrentPos[0] + gLookMargin[0] + glm::vec3(0.f, .25f, 0.f);
        glm::vec3 lightColor2(1.0f, 0.0f, 0.0f);
        glm::vec3 lightColorBulb(1.0f, 0.7f, 0.7f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 lightDir(0.0f, -0.9f, -0.17f);

        // move the light
        //angle += (float)deltaTime * 50.0f;
        //lightPos.x = 8.0f * sinf(glm::radians(angle));


        lightingShader.use();

        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("viewPos", viewPos);

        lightingShader.setUniform("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("light.diffuse", lightColor);
        lightingShader.setUniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        //lightingShader.setUniform("light.position", lightPos);
        lightingShader.setUniform("light.direction", lightDir);

        lightingShader.setUniform("light2.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setUniform("light2.diffuse", lightColor2);
        lightingShader.setUniform("light2.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setUniform("light2.position", lightPos);

        glm::mat4 I = glm::mat4(1.0f);


        if (gRenderAll) {
            for (int i = 0; i < 13; i++)
            {

                //gRocketPartsCurrentPos[i] = glm::vec3(model[3][0], model[3][1], model[3][2]);

                if (gRocketPartsCurrentVelocity[i].y > 60.f && gRocketPartsCurrentForce[i].y > 0.0f) {
                    gRocketPartsCurrentForce[i].y = 0.0f;
                    std::cout << "CAP!!!" << std::endl;
                }

                applyForces(gRocketPartsCurrentForce[i], deltaTime, i);

                gRocketPartsCurrentRotationHolder[i] += (float)deltaTime * gRocketPartsCurrentRotationAngle[i];

                model = glm::translate(I, gRocketPartsCurrentPos[i])
                    * glm::scale(I, satScale);
                model = glm::rotate(model, glm::radians(gRocketPartsCurrentRotationHolder[i]), gRocketPartsCurrentRoatationDir[i]);


                lightingShader.setUniform("model", model);

                lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
                lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
                lightingShader.setUniform("material.shininess", 150.0f);
                lightingShader.setUniformSampler("material.diffuseMap", 0);

                saturnTex.bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
                saturnV[i].draw();			// Render the OBJ mesh
                saturnTex.unbind(0);

                /*
                if (gRocketPartsCurrentForce[i].x > 0.0f) {
                    gRocketPartsCurrentForce[i].x -= 0.0001f;
                    gRocketPartsCurrentForce[i].z -= 0.0001f;
                }
                */
            }
        };

        if (gRenderPlanets) {
            model = glm::translate(glm::mat4(1.0f), moonPos) * glm::scale(glm::mat4(1.0f), moonScale);
            lightingShader.setUniform("model", model);

            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            lightingShader.setUniform("material.shininess", 10.0f);
            lightingShader.setUniformSampler("material.diffuseMap", 0);



            moonTex.bind(0);
            moon.draw();
            moonTex.unbind(0);


            model = glm::translate(glm::mat4(1.0f), earthPos) * glm::scale(glm::mat4(1.0f), earthScale);
            model = glm::rotate(model, glm::radians(60.f), glm::vec3(0.0f, 0.0f, 1.0f));
            lightingShader.setUniform("model", model);

            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            lightingShader.setUniform("material.shininess", 10.0f);
            lightingShader.setUniformSampler("material.diffuseMap", 0);



            earthTex.bind(0);
            earth.draw();
            earthTex.unbind(0);
        }

        // floor
        /*
        model = glm::translate(glm::mat4(1.0f), floorPos) * glm::scale(glm::mat4(1.0f), floorScale);
        lightingShader.setUniform("model", model);

        lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setUniform("material.shininess", 150.0f);
        lightingShader.setUniformSampler("material.diffuseMap", 0);



        floorTex.bind(0);
        floor.draw();
        floorTex.unbind(0);
        */
        // launcher
        model = glm::translate(glm::mat4(1.0f), launcherPos) * glm::scale(glm::mat4(1.0f), launcherScale);
        lightingShader.setUniform("model", model);

        lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setUniform("material.shininess", 150.0f);
        lightingShader.setUniformSampler("material.diffuseMap", 0);


        launcherTex.bind(0);
        launcher.draw();
        launcherTex.unbind(0);


        //render the light

        model = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .5f, .5f));
        lightShader.use();
        lightShader.setUniform("lightColor", lightColorBulb);

        lightShader.setUniform("model", model);
        lightShader.setUniform("view", view);
        lightShader.setUniform("projection", projection);

        lightMesh.draw();


        //SKY
        
        glDepthFunc(GL_LEQUAL);

        skyBoxShader.use();
        view = glm::mat4(glm::mat3(orbitCamera.getViewMatrix()));
        skyBoxShader.setUniform("model", glm::mat4(1.0f));
        skyBoxShader.setUniform("view", view);
        skyBoxShader.setUniform("projection", projection);

        skyBoxTex.bind(0);
        skyBox.draw();
        skyBoxTex.unbind(0);

        glDepthFunc(GL_LESS);
        

        lastTime = currentTime;

        glfwSwapBuffers(gWindow);
    }  

    glfwTerminate();
    return 0;
}
