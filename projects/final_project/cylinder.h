#ifndef CG_PROJECT_CYLINDER_H
#define CG_PROJECT_CYLINDER_H

#include <vector>

#include "../base/shader.h"
#include "../base/object3d.h"

class Cylinder:public Object3D  {
public:
    Cylinder();

    Cylinder(float base,float top,float heightin, int sector,glm::vec3 position);

    ~Cylinder();

    void draw(Shader* _cylindershader,glm::mat4 projection, glm::mat4 view) const;

    float PI=3.1415926535;

    float baseR;

    float topR;

    float height;

    int sectorNum;

    std::vector<float> _vertices;

    std::vector<float> _normals;

    std::vector<int> _indices;

    std::vector<float> unitCircleVertices;

    std::vector<unsigned int> lineIndices;

    std::vector<float> interleavedVertices;
    int interleavedStride;

    unsigned int baseIndex;                 // starting index of base
    unsigned int topIndex;

    glm::vec3 getPosition() {
        return position;
    }

    std::unique_ptr<Shader> _shader;

private:
    // vertices of the table represented in model's own coordinate

    // opengl objects
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;

    void initGLResources();

    std::vector<float> getSideNormals();

    void initShader();
};


#endif //CG_PROJECT_CYLINDER_H
