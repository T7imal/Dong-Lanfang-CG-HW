#pragma once

#include <vector>
#include <string>
#include <GL/glut.h>

class Material {
public:
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emission[4];
    float shininess;
    int textureIndex;
};

class Vertex {
public:
    float coordinates[3];
};

class TextureCoordinate {
public:
    float coordinates[2];
};

class Normal {
public:
    float vector[3];
};

class Triangle {
public:
    unsigned int vertexIndex[3];
    unsigned int textureIndex[3];
    unsigned int normalIndex[3];
};

class SubModel {
public:
    int materialIndex;
    std::vector<Triangle> triangles;
};

class Model {
public:
    Model(std::string filename);
    void load(std::string filename);
    void draw();
    void load_textures();

public:
    std::vector<std::string> textureFiles;
    GLuint* textures;
    std::vector<Material> materials;
    std::vector<Vertex> vertices;
    std::vector<TextureCoordinate> textureCoordinates;
    std::vector<Normal> normals;
    float scale[3];
    std::vector<SubModel> subModels;
};

struct Pixel {
    unsigned char r, g, b;
};

struct Image {
    int width, height;
    std::vector<Pixel> pixels;
};