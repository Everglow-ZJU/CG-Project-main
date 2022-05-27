#include "HouseBox.h"

void HouseBox::findMinAndMax(std::vector<Vertex> vertice, glm::mat4 model) {
    glm::vec4 temp;

    for (int i = 0; i < vertice.size(); i++)
    {
        temp = model*glm::vec4(vertice[i].position,1.0f) ;

        if (temp.x < minX)
        {
            minX = temp.x+40;
        }
        if (temp.x > maxX)
        {
            maxX = temp.x-40;
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
            minZ = temp.z+40;
        }
        if (temp.z > maxZ)
        {
            maxZ = temp.z-40;
        }
    }
}

bool HouseBox::checkCollision(AABBBox object) {
    bool collisionX = maxX >= object.minX && minX <= object.maxX;

    bool collisionY = maxY >= object.minY && minY <= object.maxY;

    bool collisionZ = maxZ >= object.minZ && minZ <= object.maxZ;

    if(maxX>object.maxX&&minX<object.minX&&maxY>object.maxY&&minY<object.minY&&maxZ>object.maxZ&&minZ<object.minZ)
        return false;
    else
        return collisionX && collisionY && collisionZ;
}
