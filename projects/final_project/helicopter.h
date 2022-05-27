#ifndef CG_PROJECT_HELICOPTER_H
#define CG_PROJECT_HELICOPTER_H

#include "../base/model.h"
#include "../base/light.h"
#include "../base/camera.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include "../final_project/AABBBox.h"

enum class ViewPattern {
    FPS, TPS
};

class Helicopter{
public:
    Helicopter();

    ~Helicopter();

    AABBBox* _Box;

    bool _firstDraw = true;

    void move(glm::vec3 offset);

    void draw(Shader* _heliShader,glm::mat4 projection, glm::mat4 view);

    void draw(glm::mat4 projection, glm::mat4 view,
              glm::vec3 lightPosition, glm::vec3 lightDirection, glm::vec3 viewPos);

    void initShader();

    void initShader2();

    void bindPosition(glm::vec3& position);

    void rotationX(glm::vec3 axis, float angle);

    void tiltLeft();

    void tiltRight();

    void tiltFront();

    void tiltBack();

    void resetTilt();

    int collisionDetection(glm::vec3& Normal, glm::vec3& position, glm::vec3& PlanePosition, glm::vec3& direction);

    void ViewPattern_FPS() {
        _viewPattern = ViewPattern::FPS;
    }

    void ViewPattern_TPS() {
        _viewPattern = ViewPattern::TPS;
    }

    bool isFPS() {
        return _viewPattern == ViewPattern::FPS;
    }

    glm::vec3 getPosition() {
        return _position;
    }

    glm::vec3 front() {
        return _model->getFront();
    }

    void rotate2Front(float angle);

private:
    std::unique_ptr<Model> _model;

    std::unique_ptr<Shader> _shader;

    glm::vec3 _position = { 0.0f,0.0f,800.0f };

    glm::mat4 _rotation = glm::mat4(1.0f);

    float _rotationAngle = 0.0f;

    glm::vec3 _scale = { 0.025f, 0.025f, 0.025f };

    glm::mat4 _rotation2 = glm::mat4(1.0f);

    bool _left = false;

    bool _right = false;

    bool _front = false;

    bool _back = false;

    enum ViewPattern _viewPattern = ViewPattern::TPS;

    std::unique_ptr<Texture> _texture;
};


#endif //CG_PROJECT_HELICOPTER_H
