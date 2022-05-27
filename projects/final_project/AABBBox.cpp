#include"AABBBox.h"
#include <iostream>

AABBBox::AABBBox() {
    init();
    std::cout << "Boxbuilt" << std::endl;
}

AABBBox::~AABBBox() {
    std::cout << "Boxout" << std::endl;
}

void AABBBox::init()
{
    minX = POSITIVE_INFINITY;
    maxX = NEGATIVE_INFINITY;
    minY = POSITIVE_INFINITY;
    maxY = NEGATIVE_INFINITY;
    minZ = POSITIVE_INFINITY;
    maxZ = NEGATIVE_INFINITY;
}


void AABBBox::findMinAndMax(std::vector<Vertex> vertices, glm::mat4 model) {

    glm::vec4 temp;

    for (int i = 0; i < vertices.size(); i++)
    {
        temp = model*glm::vec4(vertices[i].position,1.0f) ;

        if (temp.x < minX)
        {
            minX = temp.x;
        }
        if (temp.x > maxX)
        {
            maxX = temp.x;
        }
        if (temp.y < minY)
        {
            minY = temp.y;
        }
        if (temp.y > maxY)
        {
            maxY = temp.y;
        }
        if (temp.z < minZ)
        {
            minZ = temp.z;
        }
        if (temp.z > maxZ)
        {
            maxZ = temp.z;
        }
    }
}


bool AABBBox::checkCollision(AABBBox object) {
    bool collisionX = maxX >= object.minX && minX <= object.maxX;

    bool collisionY = maxY >= object.minY && minY <= object.maxY;

    bool collisionZ = maxZ >= object.minZ && minZ <= object.maxZ;

    return collisionX && collisionY && collisionZ;
}

void AABBBox::boxMove(glm::vec3 offset) {
    minX += offset.x; 
    maxX += offset.x; 
    minY += offset.y;
    maxY += offset.y;
    minZ += offset.z;
    maxZ += offset.z;
}