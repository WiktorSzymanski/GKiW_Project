#include "Rocket.h"

Rocket::Rocket() {
    saturnV[0].loadOBJ("../models/saturn/Saturn V_0.obj");
    saturnV[1].loadOBJ("../models/saturn/Saturn V_1.obj");
    saturnV[2].loadOBJ("../models/saturn/Saturn V_2.obj");
    saturnV[3].loadOBJ("../models/saturn/Saturn V_3.obj");
    saturnV[7].loadOBJ("../models/saturn/Saturn V_4_1.obj");
    saturnV[6].loadOBJ("../models/saturn/Saturn V_4_2.obj");
    saturnV[5].loadOBJ("../models/saturn/Saturn V_4_3.obj");
    saturnV[4].loadOBJ("../models/saturn/Saturn V_4_4.obj");
    saturnV[8].loadOBJ("../models/saturn/Saturn V_5.obj");
    saturnV[9].loadOBJ("../models/saturn/Saturn V_6.obj");
    saturnV[10].loadOBJ("../models/saturn/Saturn V_7.obj");
    saturnV[11].loadOBJ("../models/saturn/Saturn V_8.obj");
    saturnV[12].loadOBJ("../models/saturn/Saturn V_9.obj");

    saturnTex.loadTexture("../textures/Saturn_V_Texture2.png", true);
};
Rocket::~Rocket() {};

bool Rocket::getLaunch() {
	return mLaunch;
};
void Rocket::setLaunch(bool launch) {};

int Rocket::getStage() {
	return mStage;
};
void Rocket::setStage(int stage) {};

float * Rocket::getCurrentRotationAngle() {
	return mCurrentRotationAngle;
};
void Rocket::setCurrentRotationAngle(float* tab) {};

float * Rocket::getCurrentRotationHolder() {
	return mCurrentRotationHolder;
};
void Rocket::setCurrentRotationHolder(float* tab) {};

glm::vec3 * Rocket::getCurrentPos() {
	return mCurrentPos;
};
void Rocket::setCurrentPos(glm::vec3* pos) {};

glm::vec3 * Rocket::getCurrentVelocity() {
	return mCurrentVelocity;
};
void Rocket::setCurrentVelocity(glm::vec3* velocity) {};

glm::vec3 * Rocket::getCurrentForce() {
	return mCurrentForce;
};
void Rocket::setCurrentForce(glm::vec3* force) {};

glm::vec3 * Rocket::getCurrentRoatationDir() {
	return mCurrentRoatationDir;
};
void Rocket::setCurrentRoatationDir(glm::vec3* rotation) {};

void Rocket::applyForces(double deltaTime, int partIndex) {
	float m = 1;
	glm::vec3 a = mCurrentForce[partIndex] / m;

	mCurrentVelocity[partIndex] += a * (float)deltaTime;
	mCurrentPos[partIndex] += mCurrentVelocity[partIndex] * (float)deltaTime;

    mCurrentRotationHolder[partIndex] += (float)deltaTime * mCurrentRotationAngle[partIndex]; // WTF??????
};

void Rocket::applayAllForces(double deltaTime) {
    for (int i = 0; i < 13; i++) {
        applyForces(deltaTime, i);
    }
}

void Rocket::detatch() {
    switch (mStage) {
    case 1:
        mCurrentForce[12] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        mCurrentForce[11] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);

        mCurrentRotationAngle[12] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        mCurrentRotationAngle[11] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;

        break;
    case 2:
        mCurrentForce[10] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        mCurrentForce[9] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        mCurrentForce[0] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);

        mCurrentRotationAngle[10] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        mCurrentRotationAngle[9] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        mCurrentRotationAngle[0] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;

        break;
    case 3:
        mCurrentForce[8] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);
        mCurrentForce[7] = glm::vec3(.5f, -3.0f, -.5f);
        mCurrentForce[6] = glm::vec3(-.5f, -3.0f, .5f);
        mCurrentForce[5] = glm::vec3(.5f, -3.0f, .5f);
        mCurrentForce[4] = glm::vec3(-.5f, -3.0f, -.5f);
        mCurrentForce[3] = glm::vec3((float(rand()) / float((RAND_MAX))) - 0.5f, -3.0f, (float(rand()) / float((RAND_MAX))) - 0.5f);

        mCurrentRotationAngle[8] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;
        mCurrentRotationAngle[7] = 5.0f;
        mCurrentRotationAngle[6] = 5.0f;
        mCurrentRotationAngle[5] = -5.0f;
        mCurrentRotationAngle[4] = -5.0f;
        mCurrentRotationAngle[3] = (float(rand()) / float((RAND_MAX))) * 6 - 3.0f;

        break;
    }
};

void Rocket::render(ShaderProgram shader) {
    for (int i = 0; i < 13; i++)
    {
        if (mCurrentVelocity[i].y > 100.f && mCurrentForce[i].y > 0.0f) {
            mCurrentForce[i].y = 0.0f;
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), mCurrentPos[i])
            * glm::scale(glm::mat4(1.0f), satScale);
        model = glm::rotate(model, glm::radians(mCurrentRotationHolder[i]), mCurrentRoatationDir[i]);


        shader.setUniform("model", model);

        shader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        shader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setUniform("material.shininess", 150.0f);
        shader.setUniformSampler("material.diffuseMap", 0);

        saturnTex.bind(0);
        saturnV[i].draw();
        saturnTex.unbind(0);


        if (mCurrentForce[i].x > 0.0f) {
            mCurrentForce[i].x -= 0.0001f;
            mCurrentForce[i].z -= 0.0001f;
        }

        if (mCurrentForce[i].y < 0.0f) {
            mCurrentForce[i].y += 0.001f;
        }
    }
}

void Rocket::launch() {
    for (int i = 0; i < 13; i++) {
        mCurrentForce[i].y = 3.0f;
    }
};