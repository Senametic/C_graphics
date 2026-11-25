// cube.c - a spinning, colored cube using legacy (fixed-function) OpenGL + GLFW
// messing with contribution and branches

#include <GLFW/glfw3.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Replacement for gluPerspective so we don't need GLU
static void perspective(double fovy, double aspect, double zNear, double zFar) {
    double top = tan(fovy * M_PI / 360.0) * zNear;
    double right = top * aspect;
    glFrustum(-right, right, -top, top, zNear, zFar);
}

static const float vertices[8][3] = {
    {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},
    {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1},
};

static const int faces[6][4] = {
    {4, 5, 6, 7},  // front
    {1, 0, 3, 2},  // back
    {0, 4, 7, 3},  // left
    {5, 1, 2, 6},  // right
    {7, 6, 2, 3},  // top
    {0, 1, 5, 4},  // bottom
};

static const float colors[6][3] = {
    {1.0f, 0.2f, 0.2f},
    {0.2f, 1.0f, 0.2f},
    {0.2f, 0.4f, 1.0f},
    {1.0f, 1.0f, 0.2f},
    {1.0f, 0.2f, 1.0f},
    {0.2f, 1.0f, 1.0f},
};

static void draw_cube(void) {
    glBegin(GL_QUADS);
    for (int f = 0; f < 6; f++) {
        glColor3fv(colors[f]);
        for (int i = 0; i < 4; i++)
            glVertex3fv(vertices[faces[f][i]]);
    }
    glEnd();
}

int main(void) {
    if (!glfwInit()) return 1;

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Cube", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);       // vsync
    glEnable(GL_DEPTH_TEST);   // so faces occlude each other correctly

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1;

        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(60.0, (double)width / height, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);

        float t = (float)glfwGetTime();
        glRotatef(t * 50.0f, 1.0f, 1.0f, 0.0f);

        draw_cube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}