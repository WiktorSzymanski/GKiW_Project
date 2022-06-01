#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram() : mHandle(0) {};

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(mHandle);
};

bool ShaderProgram::loadShaders(const char* vsFileName, const char* fsFileName) {

    std::cout << vsFileName << std::endl;
    string vsString = fileToString(vsFileName);
    string fsString = fileToString(fsFileName);
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();

    // Creating Shaders

    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

    
    glShaderSource(VS, 1, &vsSourcePtr, NULL);
    glShaderSource(FS, 1, &fsSourcePtr, NULL);


    glCompileShader(VS);
    checkCompileErrors(VS, VERTEX);

    glCompileShader(FS);
    checkCompileErrors(FS, FRAGMENT);
    
 

    mHandle = glCreateProgram();
    glAttachShader(mHandle, VS);
    glAttachShader(mHandle, FS);
    glLinkProgram(mHandle);

    checkCompileErrors(NULL, PROGRAM);

    glDeleteShader(VS);
    glDeleteShader(FS);

    mUniformLocations.clear();

    return true;
};

void ShaderProgram::use() {
	if (mHandle > 0)
		glUseProgram(mHandle);
};

string ShaderProgram::fileToString(const string& fileName) {
	std::stringstream ss;
	std::ifstream file;

	try {
		file.open(fileName, std::ios::in);

		if (!file.fail()) {
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex) {
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
};

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type) {

    GLint status;

    if (type == PROGRAM) {
        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
            std::cerr << "Error! Program failed to ling." << errorLog << std::endl;
        }
    } else { // VERTEX of FRAGMENT
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');

            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
        }
    }
};

GLuint ShaderProgram::getProgram()const {
    return mHandle;
};

GLint ShaderProgram::getUniformLocation(const GLchar* name) {

    std::map<string, GLint>::iterator it = mUniformLocations.find(name);

    if (it == mUniformLocations.end()) {
        mUniformLocations[name] = glGetUniformLocation(mHandle, name);
    }

    return mUniformLocations[name];
};

void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v) {
    GLint loc = getUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
};

void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v) {
    GLint loc = getUniformLocation(name);
    glUniform3f(loc, v.x, v.y, v.z);
};

void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v) {
    GLint loc = getUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
};

void ShaderProgram::setUniform(const GLchar* name, const glm::mat4& m) {
    GLint loc = getUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
};

void ShaderProgram::setUniform(const GLchar* name, const GLfloat& f) {
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, f);
};

void ShaderProgram::setUniform(const GLchar* name, const GLint& i) {
    GLint loc = getUniformLocation(name);
    glUniform1f(loc, i);
};

void ShaderProgram::setUniformSampler(const GLchar* name, const GLint& slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    GLint loc = getUniformLocation(name);
    glUniform1i(loc, slot);
};