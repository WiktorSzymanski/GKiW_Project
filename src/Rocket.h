#ifndef ROCKET_H
#define ROCKET_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Mesh.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

class Rocket {
public:
	Rocket();
	~Rocket();

    void render(ShaderProgram shader);

    bool getLaunch();
    void setLaunch(bool launch);

    int getStage();
    void setStage(int stage);

    float * getCurrentRotationAngle();
    void setCurrentRotationAngle(float* tab);

    float * getCurrentRotationHolder();
    void setCurrentRotationHolder(float* tab);

    glm::vec3 * getCurrentPos();
    void setCurrentPos(glm::vec3* pos);

    glm::vec3 * getCurrentVelocity();
    void setCurrentVelocity(glm::vec3* velocity);

    glm::vec3 * getCurrentForce();
    void setCurrentForce(glm::vec3* force);

    glm::vec3 * getCurrentRoatationDir();
    void setCurrentRoatationDir(glm::vec3* rotation);

    void detatch();
    void applayAllForces(double deltaTime);

    void launch();

private:
    bool mLaunch = false;
    int mStage = 0;
    float mCurrentRotationAngle[13] = {
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
    float mCurrentRotationHolder[13] = {
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

    glm::vec3 mCurrentPos[13] = {
        glm::vec3(0.f, 22.55f ,0.f),
        glm::vec3(0.f, 22.22f ,0.f),
        glm::vec3(0.f, 20.8f ,0.f),
        glm::vec3(0.f, 20.0f ,0.f),
        glm::vec3(0.f, 20.0f ,0.f),
        glm::vec3(0.f, 20.0f ,0.f),
        glm::vec3(0.f, 20.0f ,0.f),
        glm::vec3(0.f, 20.0f ,0.f),
        glm::vec3(0.f, 16.3f ,0.f),
        glm::vec3(0.f, 16.3f ,0.f),
        glm::vec3(0.f, 11.4f ,0.f),
        glm::vec3(0.f, 11.4f ,0.f),
        glm::vec3(0.f,  3.0f ,0.f)
    };
    glm::vec3 mCurrentVelocity[13] = {
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
    glm::vec3 mCurrentForce[13] = {
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
    glm::vec3 mCurrentRoatationDir[13] = {
        glm::vec3(rand() % 2 - 1, 1.0f, rand() % 2 - 1),
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


    void applyForces(double deltaTime, int partIndex);

    Mesh saturnV[13];
    Texture2D saturnTex;

    glm::vec3 satScale = glm::vec3(0.2f, 0.2f, 0.2f);
};

#endif
