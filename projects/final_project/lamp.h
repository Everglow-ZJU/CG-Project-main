#ifndef CG_PROJECT_LAMP_H
#define CG_PROJECT_LAMP_H

#include <glm/glm.hpp>
#include "../base/shader.h"
#include<cstring>
#include <glm/ext/matrix_transform.hpp>

class Lamp{
public:
    Lamp();
    ~Lamp();
    void initShader();
    
    void draw(const glm::mat4& projection, const glm::mat4& view);

private:
    std::unique_ptr<Shader> _lightingShader;
    std::unique_ptr<Shader> _lightCubeShader;
    void cleanup();
    GLuint lightCubeVAO = 0;
	GLuint VBO = 0;
    // GLuint cubeVAO = 0;// temporary test 
    float _vertices[288];
    glm::vec3 _pointLightPositions[4];
    glm::vec3 _cubePositions[10];
};

#endif