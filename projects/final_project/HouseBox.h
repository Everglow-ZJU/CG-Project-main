#ifndef CG_PROJECT_HOUSEBOX_H
#define CG_PROJECT_HOUSEBOX_H
#include "AABBBox.h"

class HouseBox :public AABBBox{

public:
    void findMinAndMax(std::vector<Vertex> vertice, glm::mat4 model);

    bool checkCollision(AABBBox object);
};


#endif //CG_PROJECT_HOUSEBOX_H
