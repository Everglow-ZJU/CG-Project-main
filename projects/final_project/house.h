#ifndef CG_PROJECT_HOUSE_H
#define CG_PROJECT_HOUSE_H

#include <glm/glm.hpp>
#include "../base/texture.h"
#include "../base/shader.h"
#include "../base/model.h"
#include "../final_project/HouseBox.h"
class House{
public:
    House();
    ~House();

    HouseBox* _Box;

    bool _firstDraw = true;
    
    void DrawHouse(Shader* _houseshader,const glm::mat4& projection, const glm::mat4& view);
private:
    std::unique_ptr<Texture2D> _texture;
    std::unique_ptr<Model> _model;

};

#endif