#include "Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>


std::vector<std::string> split(std::string s, std::string t) {
    std::vector<std::string> res;

    while (1) {
        int pos = s.find(t);
        if (pos == -1) {
            res.push_back(s);
            break;
        }

        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1, s.size() - pos - 1);
    }

    return res;
}

Mesh::Mesh() : mLoaded(false) {};

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
};


bool Mesh::loadOBJ(const std::string& fileName) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;

    if (fileName.find(".obj") != std::string::npos) {
        std::ifstream fin(fileName, std::ios::in);
        if (!fin) {
            std::cerr << "Cannot open " << fileName << std::endl;
            return false;
        }

        std::cout << "Loading OBJ file " << fileName << " ..." << std::endl;

        std::string lineBuffer;

        while (std::getline(fin, lineBuffer)) {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v") {
                glm::vec3 vertex;

                int dim = 0;
                while (dim < 3 && ss >> vertex[dim])
                    dim++;

                tempVertices.push_back(vertex);

            } else if (cmd == "vt") {
                glm::vec2 uv;
                int dim = 0;
                while (dim < 2 && ss >> uv[dim])
                    dim++;
                tempUVs.push_back(uv);

            }
            else if (cmd == "vn") {
                glm::vec3 normal;
                int dim = 0;
                while (dim < 3 && ss >> normal[dim])
                    dim++;
                normal = glm::normalize(normal);
                tempNormals.push_back(normal);

            } else if (cmd == "f") {
                std::string faceData;
                int vertexIndex, uvIndex, normalIndex;

                while (ss >> faceData) {
                    std::vector<std::string> data = split(faceData, "/"); // v/vt/vn

                    // vertex index
                    if (data[0].size() > 0) {
                        sscanf_s(data[0].c_str(), "%d", &vertexIndex);
                        vertexIndices.push_back(vertexIndex);
                    }

                    // if the face vertex has a tex Coord index
                    if (data.size() > 1) {
                        if (data[1].size() > 0) {
                            sscanf_s(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }

                    // if the face vertex has a normal index
                    if (data.size() > 2) {
                        if (data[2].size() > 0) {
                            sscanf_s(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
            }
        }

        // Close file
        fin.close();

        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            Vertex meshVertex;

            if (tempVertices.size() > 0) {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                meshVertex.position = vertex;
            }

            if (tempNormals.size() > 0) {
                glm::vec3 normal = tempNormals[normalIndices[i] - 1];
                meshVertex.normal = normal;
            }

            if (tempUVs.size() > 0) {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                meshVertex.texCoords = uv;
            }

            mVertices.push_back(meshVertex);
        }

        initBuffers();


        std::cout << "Model " << fileName << " loaded" << std::endl;
        return (mLoaded = true);
    }

    return false;
};
void Mesh::draw() {
    if (!mLoaded) return;

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
};

void Mesh::initBuffers() {
    glGenBuffers(1, &mVBO); // crate vertex buffer object - it creates chunk of memory on graphic card
    glBindBuffer(GL_ARRAY_BUFFER, mVBO); // bind buffer as a current one, openGL can have one active buffer
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW); // sends data and the way we are going to use it to buffer

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); // telling vertex array object what sort of attributes are in array
    glEnableVertexAttribArray(0);

    // Normals attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
};