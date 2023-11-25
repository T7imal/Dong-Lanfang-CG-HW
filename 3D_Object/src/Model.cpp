#include "Model.h"
#include <fstream>


Model::Model(std::string filename) {
    load(filename);
}

void read_bmp(std::string filename, Image& img) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    char header[54];
    file.read(header, 54);

    img.width = *reinterpret_cast<int*>(&header[18]);
    img.height = abs(*reinterpret_cast<int*>(&header[22]));

    img.pixels.resize(img.width * img.height);

    int dataOffset = *reinterpret_cast<int*>(&header[10]);  // 新增：读取像素数据的开始位置
    file.seekg(dataOffset);  // 修改：将文件指针移动到像素数据的开始位置

    for (int i = 0; i < img.width * img.height; ++i) {
        file.read(reinterpret_cast<char*>(&img.pixels[i].b), 1);
        file.read(reinterpret_cast<char*>(&img.pixels[i].g), 1);
        file.read(reinterpret_cast<char*>(&img.pixels[i].r), 1);
    }
}

void Model::load_textures() {
    Image img;
    textures = new GLuint[textureFiles.size()];
    for (int i = 0; i < textureFiles.size(); i++) {
        glGenTextures(1, &textures[i]);
        read_bmp(textureFiles[i], img);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, &img.pixels[0]);
        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // 线性过滤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 线性过滤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);       // 重复贴图
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);       // 重复贴图
    }
}

void Model::load(std::string filename) {
    std::ifstream fin(filename);
    if (!fin) {
        throw std::runtime_error("Cannot open file");
    }
    int num_textures;
    int num_materials;
    int num_vertices;
    int num_texture_coordinates;
    int num_normals;
    int num_sub_models;
    std::string filepath;
    int pos = filename.find_last_of('/');
    if (pos != std::string::npos) {
        filepath = filename.substr(0, pos + 1);
    }
    fin >> num_textures;
    for (int i = 0; i < num_textures; i++) {
        std::string texture;
        fin >> texture;
        texture = filepath + texture;
        textureFiles.push_back(texture);
    }
    fin >> num_materials;
    for (int i = 0; i < num_materials; i++) {
        Material material;
        fin >> material.ambient[0] >> material.ambient[1] >> material.ambient[2] >> material.ambient[3];
        fin >> material.diffuse[0] >> material.diffuse[1] >> material.diffuse[2] >> material.diffuse[3];
        fin >> material.specular[0] >> material.specular[1] >> material.specular[2] >> material.specular[3];
        fin >> material.emission[0] >> material.emission[1] >> material.emission[2] >> material.emission[3];
        fin >> material.shininess;
        fin >> material.textureIndex;
        materials.push_back(material);
    }
    fin >> num_vertices;
    for (int i = 0; i < num_vertices; i++) {
        Vertex vertex;
        fin >> vertex.coordinates[0] >> vertex.coordinates[1] >> vertex.coordinates[2];
        vertices.push_back(vertex);
    }
    fin >> num_texture_coordinates;
    for (int i = 0; i < num_texture_coordinates; i++) {
        TextureCoordinate textureCoordinate;
        fin >> textureCoordinate.coordinates[0] >> textureCoordinate.coordinates[1];
        textureCoordinates.push_back(textureCoordinate);
    }
    fin >> num_normals;
    for (int i = 0; i < num_normals; i++) {
        Normal normal;
        fin >> normal.vector[0] >> normal.vector[1] >> normal.vector[2];
        normals.push_back(normal);
    }
    fin >> num_sub_models;
    fin >> scale[0] >> scale[1] >> scale[2];
    for (int i = 0; i < num_sub_models; i++) {
        int num_triangles;
        fin >> num_triangles;
        SubModel subModel;
        fin >> subModel.materialIndex;
        for (int j = 0; j < num_triangles; j++) {
            Triangle triangle;
            fin >> triangle.vertexIndex[0] >> triangle.textureIndex[0] >> triangle.normalIndex[0];
            fin >> triangle.vertexIndex[1] >> triangle.textureIndex[1] >> triangle.normalIndex[1];
            fin >> triangle.vertexIndex[2] >> triangle.textureIndex[2] >> triangle.normalIndex[2];
            subModel.triangles.push_back(triangle);
        }
        subModels.push_back(subModel);
    }
    fin.close();
}

void Model::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 光源
    GLfloat light_position[] = { 3.0, -3.0, 3.0, 0.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);   // 环境光
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);   // 漫反射
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // 镜面反射
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);


    for (int i = 0; i < subModels.size(); i++) {
        SubModel subModel = subModels[i];
        // 无材质
        if (subModel.materialIndex == 0) {
            glDisable(GL_TEXTURE_2D);
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < subModel.triangles.size(); j++) {
                Triangle triangle = subModel.triangles[j];
                for (int k = 0; k < 3; k++) {
                    glNormal3fv(normals[triangle.normalIndex[k] - 1].vector);
                    glVertex3fv(vertices[triangle.vertexIndex[k] - 1].coordinates);
                }
            }
            glEnd();
        }
        // 有材质
        else {
            Material material = materials[subModel.materialIndex - 1];  // 材质索引从1开始
            glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, material.specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, material.emission);
            glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
            // 无纹理
            if (material.textureIndex == 0) {
                glDisable(GL_TEXTURE_2D);
            }
            // 有纹理
            else {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, textures[material.textureIndex - 1]);
            }
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < subModel.triangles.size(); j++) {
                Triangle triangle = subModel.triangles[j];
                for (int k = 0; k < 3; k++) {
                    glTexCoord2fv(textureCoordinates[triangle.textureIndex[k] - 1].coordinates);
                    glNormal3fv(normals[triangle.normalIndex[k] - 1].vector);
                    glVertex3fv(vertices[triangle.vertexIndex[k] - 1].coordinates);
                }
            }
            glEnd();
        }

    }
    glutSwapBuffers();
}