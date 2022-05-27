#include "obj_seq.h"

const std::string objectPath = {
        "../../../media/horse/"
};

const std::string texturePath = {
        "../../../media/horse/Horse-Texture.png"
};

ObjSeq::ObjSeq() {
    animationIndex = 0;
    for (int i = 0; i < fps; i++) {
        _model[i].reset(new Model(objectPath + "/horse_obj_" + std::to_string(i) + ".obj"));
    }

    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    float rotationAngle = 3.14f;
    _rotation = glm::axisAngleMatrix(rotationAxis, rotationAngle);

    // init texture
    this->_texture.reset(new Texture2D(texturePath));

    // init shader
    initShader();

    _Box = new AABBBox();
}

void ObjSeq::initShader() {
    const char* vertCode =
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPosition;\n"
            "layout(location = 1) in vec3 aNormal;\n"
            "layout(location = 2) in vec2 aTexCoord;\n"
            "out vec2 TexCoord;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main() {\n"
            "	TexCoord = aTexCoord;\n"
            "	gl_Position = projection * view * model * vec4(aPosition, 1.0f);\n"
            "}\n";

    const char* fragCode =
            "#version 330 core\n"
            "in vec2 TexCoord;\n"
            "out vec4 color;\n"
            "uniform sampler2D mapKd;\n"
            "void main() {\n"
            "	color = texture(mapKd, TexCoord);\n"
            "}\n";

    _shader.reset(new Shader(vertCode, fragCode));
}

ObjSeq::~ObjSeq() {

}

void ObjSeq::draw(Shader* _horseshader,glm::mat4 projection, glm::mat4 view) {
    // translation
    glm::mat4 translation = glm::mat4(1.0f);
    translation = glm::translate(translation, _position);
    // rotation
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = _rotation * rotation;
    // scale
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(scale, _scale);

    glm::mat4 model = translation * rotation * scale;

    _horseshader->use();

    _horseshader->setMat4("projection", projection);
    _horseshader->setMat4("view", view);
    _horseshader->setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    _texture->bind();
    glActiveTexture(GL_TEXTURE1);
    _texture->bind();


    _model[animationIndex]->draw();

    animationIndex += 1;
    if (animationIndex == 24) {
        animationIndex = 0;
    }


}
