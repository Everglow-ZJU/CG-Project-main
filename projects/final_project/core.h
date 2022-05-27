#ifndef CG_PROJECT_CORE_H
#define CG_PROJECT_CORE_H

#pragma once

#include <memory>
#include <string>

#include "../base/application.h"
#include "../base/model.h"
#include "../base/light.h"
#include "../base/shader.h"
#include "../base/texture.h"
#include "../base/camera.h"
#include "../base/skybox.h"
#include "../final_project/house.h"
#include "../final_project/lamp.h"
#include "../final_project/helicopter.h"
#include "../final_project/door.h"
#include "../final_project/screenShot.h"
#include "../final_project/obj_seq.h"
#include"../final_project/ballModel.h"
#include"../final_project/cylinder.h"

using namespace glm;

class Core : public Application{
public:
    Core();

    ~Core();

    void renderFrame();

    void handleInput() override;

    double distance(glm::vec3  p1, glm::vec3 p2) {
        double d;
        d = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
        return d;

    }

private:
    void bindCamera2H(glm::vec3 hPosition, bool isFPS);

    void bindCamera2H(glm::vec3 hPosition, glm::vec3 axis, float angle, bool isFPS);

    bool firstBind = true;

    ObjSeq _horse;

    screenShot _screenShot;

    std::unique_ptr<SkyBox> _skybox;

    std::unique_ptr<PerspectiveCamera> _camera;

    std::unique_ptr<House> _house;

    Helicopter _helicopter;

    Door _door;

    // ballModel _ball;
    std::unique_ptr<ballModel> _ball;

    Cylinder* _cone;

    Cylinder* _cylinder;

    Cylinder* _coneFrustum;

    Cylinder* _prism;

    Cylinder* _pyramid;

    Cylinder* _frustum;





    std::unique_ptr<Lamp> _lamp;
    Shader* _lightingShader;
    Shader* _lightingShader_noTexture;
    bool rotation_flag_X = false;

    bool rotation_flag_Y = false;

    bool firstMove = true;

    bool viewChange = false;

    float rotateAngle = 0;

    struct DirLight {
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

     struct PointLight {
        vec3 position;

         float constant;
         float linear;
         float quadratic;

         vec3 ambient;
         vec3 diffuse;
         vec3 specular;
     };

    struct SpotLight {
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

};


#endif //CG_PROJECT_CORE_H
