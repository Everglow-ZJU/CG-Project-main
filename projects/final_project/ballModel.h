#ifndef CG_PROJECT_BALLMODEL_H
#define CG_PROJECT_BALLMODEL_H


#pragma once
#include "../base/model.h"
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include<vector>

class ballModel : public Object3D {
public:
    ballModel();

    ~ballModel();

    void draw(Shader* _ballshader,glm::mat4 projection, glm::mat4 view) const;

    float PI=3.1415926535;

    std::vector<float> _vertices;

    std::vector<float> _normals;

    std::vector<int> _indices;

    glm::vec3 getPosition() {
        return position;
    }

private:
    // vertices of the table represented in model's own coordinate

    // opengl objects
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;

    void initGLResources();
};


#endif //CG_PROJECT_BALLMODEL_H
