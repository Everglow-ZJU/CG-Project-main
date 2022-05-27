#pragma once
#include "../base/model.h"
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include<vector>

class Door : public Object3D {
public:
    Door();

    ~Door();

    void initShader();

    void draw(glm::mat4 projection, glm::mat4 view) const;

    void OpenDoor();

    void CloseDoor();

    float _vertices[12];

    int _indices[6];

    bool isOpen = false;

    glm::vec3 getPosition() {
        return position;
    }

private:
    // vertices of the table represented in model's own coordinate

    std::unique_ptr<Shader> _shader;

    // opengl objects
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;

    void initGLResources();
};