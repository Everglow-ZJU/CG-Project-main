
#include "cylinder.h"

Cylinder::Cylinder() {
    baseR=0;
    topR=0;
    height=0;
    sectorNum=0;
    initShader();
    initGLResources();
}

Cylinder::Cylinder(float base, float top, float heightin, int sector,glm::vec3 positionin) {

    baseR=base;
    topR=top;
    height=heightin;
    sectorNum=sector;

    scale = glm::vec3(5.0f, 5.0f, 5.0f);
    position = positionin;


    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorNum;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for(int i = 0; i <= sectorNum; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    float x, y, z;                                  // vertex position
    //float s, t;                                     // texCoord
    float radius;                                   // radius for each stack

    // get normals for cylinder sides
    std::vector<float> sideNormals = getSideNormals();

    // put vertices of side cylinder to array by scaling unit circle
    for(int i = 0; i <= 10; ++i)
    {
        z = -(height * 0.5f) + (float)i / 10 * height;      // vertex position z
        radius = baseR + (float)i / 10 * (topR - baseR);     // lerp
        float t = 1.0f - (float)i / 10;   // top-to-bottom

        for(int j = 0, k = 0; j <= sectorNum; ++j, k += 3)
        {
            x = unitCircleVertices[k];
            y = unitCircleVertices[k+1];

            _vertices.push_back(x * radius);
            _vertices.push_back(y * radius);
            _vertices.push_back(z);
            //normal
            _vertices.push_back(sideNormals[k]);
            _vertices.push_back(sideNormals[k+1]);
            _vertices.push_back(sideNormals[k+2]);

        }
    }

    // remember where the base.top vertices start
    unsigned int baseVertexIndex = (unsigned int)_vertices.size() / 6;

    // put vertices of base of cylinder
    z = -height * 0.5f;
    _vertices.push_back(0);
    _vertices.push_back(0);
    _vertices.push_back(z);
//normal
    _vertices.push_back(0);
    _vertices.push_back(0);
    _vertices.push_back(-1);

    for(int i = 0, j = 0; i < sectorNum; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        _vertices.push_back(x * baseR);
        _vertices.push_back(y * baseR);
        _vertices.push_back(z);
//normal
        _vertices.push_back(0);
        _vertices.push_back(0);
        _vertices.push_back(-1);
    }

    // remember where the base vertices start
    unsigned int topVertexIndex = (unsigned int)_vertices.size() / 6;

    // put vertices of top of cylinder
    z = height * 0.5f;
    _vertices.push_back(0);
    _vertices.push_back(0);
    _vertices.push_back(z);
//normal
    _vertices.push_back(0);
    _vertices.push_back(0);
    _vertices.push_back(1);
    for(int i = 0, j = 0; i < sectorNum; ++i, j += 3)
    {
        x = unitCircleVertices[j];
        y = unitCircleVertices[j+1];
        _vertices.push_back(x * topR);
        _vertices.push_back(y * topR);
        _vertices.push_back(z);
//normal
        _vertices.push_back(0);
        _vertices.push_back(0);
        _vertices.push_back(1);
    }

    // put indices for sides
    unsigned int k1, k2;
    for(int i = 0; i < 10; ++i)
    {
        k1 = i * (sectorNum + 1);     // bebinning of current stack
        k2 = k1 + sectorNum + 1;      // beginning of next stack

        for(int j = 0; j < sectorNum; ++j, ++k1, ++k2)
        {
            // 2 trianles per sector

            _indices.push_back(k1);
            _indices.push_back(k1+1);
            _indices.push_back(k2);

            _indices.push_back(k2);
            _indices.push_back(k1+1);
            _indices.push_back(k2+1);

            // vertical lines for all stacks
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            // horizontal lines
            lineIndices.push_back(k2);
            lineIndices.push_back(k2 + 1);
            if(i == 0)
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    // remember where the base indices start
    baseIndex = (unsigned int)_indices.size();

    // put indices for base
    for(int i = 0, k = baseVertexIndex + 1; i < sectorNum; ++i, ++k)
    {
        if(i < (sectorNum - 1)){
            _indices.push_back(baseVertexIndex);
            _indices.push_back(k+1);
            _indices.push_back(k);
        }
        else {
            _indices.push_back(baseVertexIndex);
            _indices.push_back(baseVertexIndex+1);
            _indices.push_back(k);// last triangle
        }
    }

    // remember where the base indices start
    topIndex = (unsigned int)_indices.size();

    for(int i = 0, k = topVertexIndex + 1; i < sectorNum; ++i, ++k)
    {
        if(i < (sectorNum - 1)){
            _indices.push_back(topVertexIndex);
            _indices.push_back(k);
            _indices.push_back(k+1);
        }
        else{
            _indices.push_back(topVertexIndex);
            _indices.push_back(k);
            _indices.push_back(topVertexIndex+1);
        }
    }

    initShader();
    initGLResources();
}

std::vector<float> Cylinder::getSideNormals() {
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectorNum;
    float sectorAngle;  // radian

    // compute the normal vector at 0 degree first
    // tanA = (baseRadius-topRadius) / height
    float zAngle = atan2(baseR - topR, height);
    float x0 = cos(zAngle);     // nx
    float y0 = 0;               // ny
    float z0 = sin(zAngle);     // nz

    // rotate (x0,y0,z0) per sector angle
    std::vector<float> normals;
    for(int i = 0; i <= sectorNum; ++i)
    {
        sectorAngle = i * sectorStep;
        normals.push_back(cos(sectorAngle)*x0 - sin(sectorAngle)*y0);   // nx
        normals.push_back(sin(sectorAngle)*x0 + cos(sectorAngle)*y0);   // ny
        normals.push_back(z0);  // nz
    }

    return normals;
}

void Cylinder::initGLResources() {
    // interleaved array
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

void Cylinder::draw(Shader* _cylindershader,glm::mat4 projection, glm::mat4 view) const {
    _cylindershader->use();
    _cylindershader->setMat4("projection", projection);
    _cylindershader->setMat4("view", view);
    _cylindershader->setMat4("model", getModelMatrix());
    _cylindershader->setVec3("material.diffuse", glm::vec3( 0.0f, 0.5f, 1.0f));
    _cylindershader->setVec3("material.specular", glm::vec3( 1.0f, 1.0f, 1.0f));
    _cylindershader->setFloat("material.shininess", 32.0f);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (unsigned int)_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

Cylinder::~Cylinder() {

}




void Cylinder::initShader() {
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
