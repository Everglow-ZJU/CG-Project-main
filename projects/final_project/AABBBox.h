#ifndef CG_PROJECT_AABBOX_H
#define CG_PROJECT_AABBOX_H

#include "../base/vertex.h"
#include<vector>
#include<stdio.h>
#include <string>
#include <glad/glad.h>


#define NEGATIVE_INFINITY -1000000000000000.0

#define POSITIVE_INFINITY 1000000000000000.0
class AABBBox {
public:

    AABBBox();

    ~AABBBox();

    void init();

    void findMinAndMax(std::vector<Vertex> vertice, glm::mat4 model);

    bool checkCollision(AABBBox object);

    void boxMove(glm::vec3 offset);

    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;

private:


};

#endif CG_PROJECT_AABBOX_H