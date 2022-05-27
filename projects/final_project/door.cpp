
#include"door.h"

Door::Door() {
    scale = glm::vec3(300.0f, 300.0f, 300.0f);
    position = glm::vec3(-230.0f, -46.5f, 360.0f);


    float vertices[] = {
            0.6f, 0.7f, 0.0f,
            0.6f, -0.7f, 0.0f,
            -0.0f, -0.7f, 0.0f,
            -0.0f, 0.7f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    for (int i = 0; i < 12; i++) {
        _vertices[i] = vertices[i];
    }
    for (int i = 0; i < 6; i++) {
        _indices[i] = indices[i];
    }


    initGLResources();

    initShader();

    // init shader

}

Door::~Door() {

}

void Door::initShader() {
    const char* vertCode = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "uniform mat4 model;\n"
                           "uniform mat4 view;\n"
                           "uniform mat4 projection;\n"
                           "void main()\n"
                           "{\n"
                           "   gl_Position = projection * view * model*vec4(aPos, 1.0);\n"
                           "}\0";
    const char* fragCode = "#version 330 core\n"
                           "out vec4 FragColor;\n"
                           "void main()\n"
                           "{\n"
                           "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                           "}\n\0";

    _shader.reset(new Shader(vertCode, fragCode));
}


void Door::initGLResources() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Door::draw(glm::mat4 projection, glm::mat4 view) const {


    _shader->use();
    _shader->setMat4("projection", projection);
    _shader->setMat4("view", view);
    _shader->setMat4("model", getModelMatrix());

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Door::OpenDoor() {

    glm::vec3 rotationMatrix = glm::vec3(0.0f, -90.0f, 0.0f);
    rotation = rotationMatrix;

    isOpen = true;

}

void Door::CloseDoor() {

    glm::vec3 rotationMatrix = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = rotationMatrix;

    isOpen = false;

}