#ifndef CG_PROJECT_SCREENSHOT_H
#define CG_PROJECT_SCREENSHOT_H

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include <GLFW/glfw3.h>


class screenShot {
public:
    screenShot();

    ~screenShot();

    bool function(int g_gl_width, int g_gl_height);
};


#endif //CG_PROJECT_SCREENSHOT_H
