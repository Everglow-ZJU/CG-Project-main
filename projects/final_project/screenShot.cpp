#include "screenShot.h"

screenShot::screenShot() {

}

screenShot::~screenShot() {

}

bool screenShot::function(int g_gl_width, int g_gl_height) {
    // return false;
    auto * buffer = (unsigned char*)malloc (g_gl_width * g_gl_height * 3);
    glReadPixels (0, 0, g_gl_width, g_gl_height, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    char name[1024];
    long int t = time (nullptr);
    sprintf (name, "../../../screenshots/screenshot_%ld.png", t);
    unsigned char* last_row = buffer + (g_gl_width * 3 * (g_gl_height - 1));
    if (!stbi_write_png (name, g_gl_width, g_gl_height, 3, last_row, -3 * g_gl_width)) {
        fprintf (stderr, "ERROR: could not write screenshot file %s\n", name);
    }
    free (buffer);
    return true;
}
