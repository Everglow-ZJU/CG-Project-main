#ifndef CG_PROJECT_OBJ_SEQ_H
#define CG_PROJECT_OBJ_SEQ_H

#include "../base/model.h"
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include "../final_project/AABBBox.h"

class ObjSeq {
public:
    ObjSeq();

    ~ObjSeq();

    void draw(Shader* _horseshader,glm::mat4 projection, glm::mat4 view);

    AABBBox* _Box;

private:
    void initShader();

    int fps = 24;

    int animationIndex;

    std::unique_ptr<Model> _model[24];

    std::unique_ptr<Shader> _shader;

    glm::vec3 _position = { 1000.0f,-500.0f,600.0f };

    glm::mat4 _rotation = glm::mat4(1.0f);

    float _rotationAngle = 0.0f;

    glm::vec3 _scale = { 1000.f, 1000.f, 1000.f };

    std::unique_ptr<Texture> _texture;


};


#endif //CG_PROJECT_OBJ_SEQ_H
