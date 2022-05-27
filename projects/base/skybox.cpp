#include "skybox.h"

SkyBox::SkyBox(const std::vector<std::string>& textureFilenames) {
	GLfloat vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
	};

    // create vao and vbo
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glBindVertexArray(0);

    try {
        // init texture
        _texture.reset(new TextureCubemap(textureFilenames));

        const char* vertCode =
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "out vec3 texCoord;\n"
            "uniform mat4 projection;\n"
            "uniform mat4 view;\n"
            "void main() {\n"
            "   texCoord = aPosition;\n"
            "   gl_Position = (projection * view * vec4(aPosition, 1.0f)).xyww;\n"
            "}\n";

        const char* fragCode =
            "#version 330 core\n"
            "out vec4 color;\n"
            "in vec3 texCoord;\n"
            "uniform samplerCube cubemap;\n"
            "void main() {\n"
            "   color = texture(cubemap, texCoord);\n"
            "}\n";

        _shader.reset(new Shader(vertCode, fragCode));
    } catch(const std::exception&) {
        cleanup();
        throw;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::stringstream ss;
        ss << "skybox creation failure, (code " << error << ")";
        cleanup();
        throw std::runtime_error(ss.str());
    }
}

SkyBox::~SkyBox() {
    cleanup();
}

void SkyBox::draw(const glm::mat4& projection, const glm::mat4& view) {

    glm::mat4 view1 = glm::mat4(glm::mat3(view));

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    _shader->use();
    _shader->setMat4("view", view1);
    _shader->setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(_vao);
    glActiveTexture(GL_TEXTURE0);
    _texture->bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default

}

void SkyBox::cleanup() {
    if (_vbo != 0) {
        glDeleteBuffers(1, &_vbo);
        _vbo = 0;
    }

    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}