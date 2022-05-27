#include "ballModel.h"

ballModel::ballModel() {
    scale = glm::vec3(20.0f, 20.0f, 20.0f);
    position = glm::vec3(-230.0f, -46.5f, 200.0f);

    for (int y=0;y<=50;y++)
    {
        for (int x=0;x<=50;x++)
        {
            float xSegment = (float)x / (float)50;//u
            float ySegment = (float)y / (float)50;//v
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            _vertices.push_back(xPos);
            _vertices.push_back(yPos);
            _vertices.push_back(zPos);

            _vertices.push_back(xPos);
            _vertices.push_back(yPos);
            _vertices.push_back(zPos);
        }
    }

    //Create ball's indicate
    for (int i=0;i<50;i++)
    {
        for (int j=0;j<50;j++)
        {
            _indices.push_back(i * (50 + 1) + j);
            _indices.push_back((i + 1) * (50 + 1) + j);
            _indices.push_back((i + 1) * (50 + 1) + j+1);
            _indices.push_back(i* (50 + 1) + j);
            _indices.push_back((i + 1) * (50 + 1) + j + 1);
            _indices.push_back(i * (50 + 1) + j + 1);
        }
    }


    initGLResources();

    // initShader();
}

ballModel::~ballModel() {

}

void ballModel::draw(Shader* _ballshader,glm::mat4 projection, glm::mat4 view) const {
    _ballshader->use();
    _ballshader->setMat4("projection", projection);
    _ballshader->setMat4("view", view);
    _ballshader->setMat4("model", getModelMatrix());
    _ballshader->setVec3("material.diffuse", glm::vec3( 0.0f, 1.0f, 0.0f));
    _ballshader->setVec3("material.specular", glm::vec3( 1.0f, 1.0f, 1.0f));
    _ballshader->setFloat("material.shininess", 32.0f);
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 50*50*6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ballModel::initGLResources() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(float), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size()*sizeof(int), &_indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
