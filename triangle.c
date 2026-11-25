// triangle.c - a spinning, colored triangle using legacy (fixed-function) OpenGL + GLFW
// make using   gcc triangle.c -o triangle.exe -lglfw3 -lopengl32
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

static const float vertices[3][3] = {
    {-1.0f, -1.0f, 0.0f},
    { 1.0f, -1.0f, 0.0f},
    { 0.0f,  1.0f, 0.0f},
};

static const float colors[3][3] = {
    {1.0f, 0.2f, 0.2f},
    {0.2f, 1.0f, 0.2f},
    {0.2f, 0.4f, 1.0f},
};

static void draw_triangle(void) {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        glColor3fv(colors[i]);
        glVertex3fv(vertices[i]);
    }
    glEnd();
}

int main(void) {
    if (!glfwInit()) return 1;

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
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

        draw_triangle();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}