#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "GL/glew.h"
#include <string>
#include <vector>
using std::string;

class Texture3D {
public:
	Texture3D();
	virtual ~Texture3D();

	bool loadTexture(std::vector<string> faces);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);
private:
	GLuint mTexture;
};

#endif